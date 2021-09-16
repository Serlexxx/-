#include "Painter.h"

Point point;
/**
* \brief Создание буфера кадра (двумерный массив структур RGBQUAD)
* \param width Ширина буфера кадра
* \param height Высота буфера кадра
* \param clearColor Изначальный цвет
* \return Указатель на массив
*/
RGBQUAD* CreateFrameBuffer(uint32_t width, uint32_t height, RGBQUAD clearColor)
{
	if (width != 0 && height != 0) {
		RGBQUAD* frame = new RGBQUAD[width * height];
		ClearFrame(frame, width * height, clearColor);

		return frame;
	}
}

/**
* \brief Заполнение буфера изображения каким-то конкретным цветом
* \param buffer Буфер кадра (указатель на массив)
* \param pixelCount Кол-во пикселей в буфере
* \param clearColor Цвет
*/
void ClearFrame(RGBQUAD* buffer, uint32_t pixelCount, RGBQUAD clearColor)
{
	std::fill_n(buffer, pixelCount, clearColor);
}

/**
* \brief Установка пикселя
* \param buffer Буфер кадра (указатель на массив)
* \param x Положение по оси X
* \param y Положение по ост Y
* \param w Ширина фрейм-буфера
* \param color Очистка цвета
*/
void SetPoint(RGBQUAD* buffer, int x, int y, uint32_t w, RGBQUAD color)
{

	buffer[(y * w) + x] = color;
}



/**
* \brief Рисование линии (быстрый вариант, алгоритм Брэзенхема)
* \param buffer Буфер кадра (указатель на массив)
* \param x0 Начальная точка (компонента X)
* \param y0 Начальная точка (компонента Y)
* \param x1 Конечная точка (компонента X)
* \param y1 Конечная точка (компонента Y)
* \param w Ширина фрейм-буфера
* \param color Очистка цвета
*/
void SetLine(RGBQUAD* buffer, uint32_t w, RGBQUAD color)
{
	int j = 0;
	for (int i = 0; i < point.setApex(); i++) {
		i < (point.setApex() - 1) ? j++ : j = 0;
		double const deltaX = abs(point.pointer[i].x - point.pointer[j].x);
		double const deltaY = abs(point.pointer[i].y - point.pointer[j].y);
		double x_1, x_2, y_1, y_2;
		double x, y, xend, yend, s, d, inc1, inc2;
		double check_x, check_y;
		x_1 = point.pointer[i].x;
		x_2 = point.pointer[j].x;
		y_1 = point.pointer[i].y;
		y_2 = point.pointer[j].y;

		if (deltaX > deltaY) {
			inc1 = 2 * deltaY;
			inc2 = 2 * (deltaY - deltaX);
			d = 2 * deltaY - deltaX;
			if (x_1 < x_2) {
				x = x_1; y = y_1; xend = x_2;
				(y_1 < y_2) ? s = 1 : s = -1;
			}
			else {
				x = x_2;
				y = y_2;
				xend = x_1;
				(y_1 > y_2) ? s = 1 : s = -1;
			}

			SetPoint(buffer, x, y, w, color);
			while (x < xend) {
				x++;
				if (d > 0) {
					y += s;
					d += inc2;
				}
				else
					d += inc1;

				SetPoint(buffer, x, y, w, color);
			}
		}
		else {
			inc1 = 2 * deltaX;
			inc2 = 2 * (deltaX - deltaY);
			d = 2 * deltaX - deltaY;
			if (y_1 < y_2) {
				y = y_1;
				x = x_1;
				yend = y_2;
				(x_1 < x_2) ? s = 1 : s = -1;
			}
			else {
				y = y_2;
				x = x_2;
				yend = y_1;
				(x_1 > x_2) ? s = 1 : s = -1;
			}

			SetPoint(buffer, x, y, w, color);
			while (y < yend) {
				y++;
				if (d > 0) {
					x += s;
					d += inc2;
				}
				else
					d += inc1;

				SetPoint(buffer, x, y, w, color);
			}

		}
	}
}

/**
* \param k  Коэффициент масштабирования линии
* \param height Высота рабочей области
* \param width Ширина рабочей области
* */
void MatrixMultiplication(double k, double height, double width)
{
	int check = 0;
	struct Prod
	{
		double x, y;
	};

	Prod* ResProd = new Prod[point.setApex()];
	int diag = 0;
	double cnt_apex = point.setApex();
	double Dx, Dy;

	if (point.setApex() % 2 == 1) {
		while (cnt_apex != 1) {
			cnt_apex -= 2;
			diag++;
		}
		double Dx_0 = (point.pointer[0].x + point.pointer[1].x) / 2;
		double Dy_0 = (point.pointer[0].y + point.pointer[1].y) / 2;
		Dx = (point.pointer[diag].x + Dx_0) / 2;
		Dy = (point.pointer[diag].y + Dy_0) / 3;
	}
	else {
		Dx = (point.pointer[0].x + point.pointer[point.setApex() / 2].x) / 2;
		Dy = (point.pointer[0].y + point.pointer[point.setApex() / 2].y) / 2;
	}
	for (int cnt = 0; cnt < point.setApex(); cnt++) {


		double aMatrix[1][3] = { {point.pointer[cnt].x, point.pointer[cnt].y, 1} };
		double bMatrix[3][3] = { {k, 0, 0}, {0, k, 0}, {0, 0, 1} };
		double product[1][3] = { {0, 0, 0} };
		for (int i = 0; i < 1; i++) {
			for (int j = 0; j < 3; j++) {
				for (int inner = 0; inner < 3; inner++) {
					product[i][j] += floor(aMatrix[i][inner] * bMatrix[inner][j]);
				}
				//std::cout << product[i][j] << "  ";
			}
			//std::cout << std::endl;
		}
		ResProd[cnt].y = product[0][1];
		ResProd[cnt].x = product[0][0];

		/*
		if ((abs(product[0][1]) > 10) && (abs(product[0][1]) < (height - 10)) && (abs(product[0][0]) > 10) && (abs(product[0][0]) < (width - 10))) {
			check++;
			ResProd[cnt].y = product[0][1];
			ResProd[cnt].x = product[0][0];
		}
	}
	if (check == APEX) {
		for (int i = 0; i < APEX; i++) {
			std::cout << ResProd[i].x << " x " << ResProd[i].y << std::endl;
			pointer[i].x = ResProd[i].x;
			pointer[i].y = ResProd[i].y;
		}
	}
	else {
		std::cout << "EROR: Border Reached" << std::endl;
		for (int i = 0; i < APEX; i++) {
			std::cout << ResProd[i].x << " x " << ResProd[i].y << std::endl;
		}
		std::cout << std::endl;
	}
	*/
	}
	for (int i = 0; i < point.setApex(); i++) {
		std::cout << ResProd[i].x << " x " << ResProd[i].y << std::endl;
		point.pointer[i].x = ResProd[i].x;
		point.pointer[i].y = ResProd[i].y;
	}
}

/**
* \param angle  Коэффициент поворота линии
* \param height Высота рабочей области
* \param width Ширина рабочей области
* */
void Rotate(double angle, double height, double width)
{
	int cnt_apex = point.setApex();
	int diag = 0;
	double Dx, Dy;

	if (point.setApex() % 2 == 1) {
		while (cnt_apex != 1) {
			cnt_apex -= 2;
			diag++;
		}
		double Dx_0 = (point.pointer[0].x + point.pointer[1].x) / 2;
		double Dy_0 = (point.pointer[0].y + point.pointer[1].y) / 2;
		Dx = (point.pointer[diag].x + Dx_0) / 2;
		Dy = (point.pointer[diag].y + Dy_0) / 3;
	}
	else {
		Dx = (point.pointer[0].x + point.pointer[point.setApex() / 2].x) / 2;
		Dy = (point.pointer[0].y + point.pointer[point.setApex() / 2].y) / 2;
	}

	struct RadPoints
	{
		double xR, yR;
	};
	RadPoints *massiv = new RadPoints[point.setApex()];
	int check = 0;

	for (int i = 0; i < point.setApex(); i++) {
		double x = point.pointer[i].x;
		double y = point.pointer[i].y;
		//massiv[i].xR 
		point.pointer[i].x = Dx + (x - Dx) * cos(angle) - (y - Dy) * sin(angle);
		//massiv[i].yR 
		point.pointer[i].y = Dy + (x - Dx) * sin(angle) + (y - Dy) * cos(angle);
		/*
		if ((abs(massiv[i].yR) > 10) && (abs(massiv[i].yR) < abs(width - 10)) && (abs(massiv[i].xR) > 10) && (abs(massiv[i].xR) < abs(height - 10))) {
			check++;
		}
		*/
	}
	/*if (check == APEX) {
		for (int i = 0; i < APEX; i++)
		{
			pointer[i].x = massiv[i].xR;
			pointer[i].y = massiv[i].yR;
		}
	}
	else {
		std::cout << "EROR: Border Reached" << std::endl;
	}*/
}


/*
* Заливает строку iy от ixn до ixk
*/
void FILSTR(RGBQUAD* buffer, uint32_t w, int iy, int ixn, int ixk, RGBQUAD color) {
	while (ixn <= ixk) SetPoint(buffer, ixn++, iy, w, color);
} /* FILSTR */

static double* pt;

static int NextApex;	/* Номер следующей вершины в списке */
double* yUP = new double[point.setApex()];	/*Y-коориданты вершин по возрастанию */
int* yNumb = new int[point.setApex()]; /* их номера */

/*Список активных ребер*/
static int IDLSPI;			/*Длина списка активных ребер*/
static int IYREB[300];		/*Макс Y-коорд активных ребер*/
static double RXREB[300];	/*Тек X-коорд акивных ребер*/
static double RPRIP[300];	/*X-приращение на 1 шаг по Y*/
static double RYSL[300];	/*Dy между тек и соседн верш */
							/*Dy <= 0.0 - обычная вершина */
							/*	  > 0.0 - локал эксремум */

static int FORSPI(RGBQUAD* buffer, uint32_t w, int IYBEG, RGBQUAD color) {
	int i, ikledg, intek, intabs, isd;
	int iyt, ixt, nrebra, inc, inpred, inposl;
	double xt, xc, yt, yc, dy;

	ikledg = 0;
	for (i = NextApex; i < point.setApex(); ++i)
		if (yUP[i] != IYBEG) break; else ikledg++;

	for (i = 0; i < ikledg; i++) {
		intek = yNumb[NextApex + i];
		intabs = abs(intek);
		xt = pt[intabs - 1];
		yt = pt[intabs];

		if ((inpred = intabs - 2) < 0) inpred = yNumb[(point.setApex() - 1)]; //правка - APEX*2-1
		if ((inposl = intabs + 2) > (point.setApex() * 2 - 1)) inposl = yNumb[0]; //правка - 1

		for (isd = 0; isd < 2; isd += 2) {
			if (!isd) nrebra = inc = inpred;
			else {
				inc = inposl; nrebra = intabs;
			}
			yc = pt[inc];
			dy = yc - yt;
			if (dy < 0.0) continue;
			xc = pt[inc - 1];
			int help = floor(nrebra / 2);
			if (dy != 0.0) goto DYNE0;

			if ((inc = yNumb[help]) < 0) continue;
			yNumb[help] = -inc;
			iyt = yt;
			inc = xc;
			ixt = xt;
			FILSTR(buffer, w, iyt, ixt, inc, color);
			continue;
		DYNE0:
			IYREB[IDLSPI] = yc;
			RXREB[IDLSPI] = xt;
			RPRIP[IDLSPI] = (xc - xt) / dy;
			inc = (!isd) ? inposl : inpred;
			RYSL[IDLSPI] = pt[inc] - yt;
			++IDLSPI;
		}
	}
	if ((i = (NextApex += ikledg)) > (point.setApex() - 1)) i = (point.setApex() - 1);
	return(yUP[i]);
}

/*
* Однотонно заливает многоугольник,
* заданный координатами вершин
*/
void V_FP1(RGBQUAD* buffer, uint32_t w, double* Pt, RGBQUAD color) {
	int i, j, k, l;
	int iytek = 0;
	int iymin;
	int iybeg;
	int iymak;
	int iysled;
	int newysl;
	int ixmin = 0;
	int ixtek;
	int irabx[300];

	memset(irabx, 0, 300 * 4);
	pt = Pt;
	/*Построение массивов У и их номеров*/
	int cnt = 0;
	for (i = 1; i < (point.setApex() * 2); i += 2) {
		yUP[cnt] = *(pt + i);
		yNumb[cnt] = i;
		cnt++;
	}
	/*Сортировка*/

	bool sort_or_not = 1;

	do {
		sort_or_not = 1;
		for (int i = 0; i < (point.setApex() - 1); i++) { // n - размер сортируемого массива
			if (yUP[i] > yUP[i + 1]) {
				swaping(yUP[i], yUP[i + 1]);
				swaping_int(yNumb[i], yNumb[i + 1]);
				sort_or_not = 1;
			}
		}
		for (int i = (point.setApex() - 1); i >= 1; i--) {
			if (yUP[i] < yUP[i - 1]) {
				swaping(yUP[i], yUP[i - 1]);
				swaping_int(yNumb[i], yNumb[i - 1]);
				sort_or_not = 0;
			}
		}
	} while (sort_or_not == 0);

	/*
	for (i = 0; i < APEX; ++i) {
		iymin = yUP[i];
		k = 0;
		for(j = i + 1; j < APEX; ++j)
			if ((l = yUP[i]) < iymin) {
				iymin = l; k = j;
			}
		if (!k) {
			yUP[k] = yUP[i]; yUP[i] = iymin;
			iymin = yNumb[k];
			yNumb[k] = yNumb[i]; yNumb[i] = iymin;
		}
	}
	*/
	/*Начальные присвоения*/
	IDLSPI = 0;
	NextApex = 0;
	iybeg = yUP[0];
	iymak = yUP[point.setApex() - 1];

	iysled = FORSPI(buffer, w, iybeg, color);
	if (!IDLSPI) goto KOHGFA;

ZALIWKA:
	for (iytek = iybeg; iytek <= iysled; ++iytek) {
		if (iytek == iysled) {
			newysl = FORSPI(buffer, w, iytek, color);
			if (!IDLSPI) goto KOHGFA;
		}
		l = 0;
		for (i = 0; i < IDLSPI; ++i)
			if (RYSL[i] > 0.0) { irabx[l] = RXREB[i]; l++; }
			else RYSL[i] = 1.0;
		for (i = 0; i < l; ++i) {
			ixmin = irabx[i];
			k = 0;
			for (j = i; j < l; ++j) {
				ixtek = irabx[j];
				if (ixtek < ixmin) { k = j; ixmin = ixtek; }
			}
			if (k) { irabx[k] = irabx[i]; irabx[i] = ixmin; }
		}
		for (j = 0; j <= l - 1; j += 2)
			FILSTR(buffer, w, iytek, irabx[j], irabx[j + 1], color);
		for (j = 0; j <= IDLSPI; ++j)
			RXREB[j] = RXREB[j] + RPRIP[j];
	}
	if (iysled == iymak) goto KOHGFA;

	i = 0;
M1: ++i;
M2: if (i > IDLSPI) goto WYBROSILI;
	if (IYREB[i] != iysled) goto M1;
	--IDLSPI;
	for (j = i; j <= IDLSPI; ++j) {
		IYREB[j] = IYREB[k = j + 1];
		RXREB[j] = RXREB[k];
		RPRIP[j] = RPRIP[k];
	}
	goto M2;
WYBROSILI:
	iybeg = iysled + 1;
	iysled = newysl;
	goto ZALIWKA;
KOHGFA:;
}

void swaping(double& one, double& two) {
	double three = one;
	one = two;
	two = three;
}
void swaping_int(int& one, int& two) {
	int three = one;
	one = two;
	two = three;
}
