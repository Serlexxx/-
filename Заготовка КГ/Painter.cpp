#include "Painter.h"
using namespace std;
#define PI 3.14159265


/**
* \brief Установка пикселя
* \param buffer Буфер кадра (указатель на массив)
* \param x Положение по оси X
* \param y Положение по ост Y
* \param w Ширина фрейм-буфера
* \param color Очистка цвета
*/


void SetPoint(BitmapBuffer* buffer, int x, int y, BitmapRGB color)
{
	if (y < static_cast<int>(buffer->GetHeight()) &&
		y > 0 && x < static_cast<int>(buffer->GetWidth()) && x > 0)
	{
		(*buffer)[y][x].red = color.red;
		(*buffer)[y][x].green = color.green;
		(*buffer)[y][x].blue = color.blue;
	}
}



/**
 * \brief Отрисовка кадра
 * \param width Ширина
 * \param height Высота
 * \param pixels Массив пикселов
 * \param hWnd Хендл окна, device context которого будет использован
 */
void PresentFrame( uint32_t width, uint32_t height, void* pixels, HWND hWnd)
{
	// Получить хендл на временный bit-map (4 байта на пиксель)
	HBITMAP hBitMap = CreateBitmap(width, height, 1, 8 * 4, pixels);

	// Получить device context окна
	HDC hdc = GetDC(hWnd);

	// Временный DC для переноса bit-map'а
	HDC srcHdc = CreateCompatibleDC(hdc);

	// Связать bit-map с временным DC
	SelectObject(srcHdc, hBitMap);

	// Копировать содержимое временного DC в DC окна
	BitBlt(
		hdc,    // HDC назначения
		0,      // Начало вставки по оси X
		0,      // Начало вставки по оси Y
		width,  // Ширина
		height, // Высота
		srcHdc, // Исходный HDC (из которого будут копироваться данные)
		0,      // Начало считывания по оси X
		0,      // Начало считывания по оси Y
		SRCCOPY // Копировать //было SRCCOPY, поменял на SRCAND
	);
	//Sleep(10000);
	// Уничтожить bit-map
	DeleteObject(hBitMap);
	// Уничтодить временный DC
	DeleteDC(srcHdc);
	// Уничтодить DC
	DeleteDC(hdc);
	ReleaseDC(hWnd, hdc);
	//PostQuitMessage(0);
}

/*
* Двойной брензехем для 3D
*/

void PutLine3D(BitmapBuffer* buffer, PointBuffer& pointer, int i, int i1,  BitmapRGB color)
{ //
	int dx = abs(pointer[i1].x - pointer[i].x); // длины путей по коорд. осям
	int dy = abs(pointer[i1].y - pointer[i].y);
	int dz = abs(pointer[i1].z - pointer[i].z); // NB: dx>=dy>=dz !

	int sx = (pointer[i1].x >= pointer[i].x) ? 1 : -1; // единичные направления движения
	int sy = (pointer[i1].y >= pointer[i].y) ? 1 : -1;
	int sz = (pointer[i1].z >= pointer[i].z) ? 1 : -1;

	int d1, d2, d;
	int dd1, dd2, dd;
	int x, y, z, j;

	//SetPoint(buffer, pointer[i].x, pointer[i].y, color); // вывод первой точки

	d = (dy << 1) - dx;  // инициализация пары 'x'-'y'
	d1 = dy << 1;
	d2 = (dy - dx) << 1;

	dd = (dz << 1) - dy; // инициализация пары 'y'-'z'
	dd1 = dz << 1;
	dd2 = (dz - dy) << 1;

	x = pointer[i].x + sx;
	y = pointer[i].y;
	z = pointer[i].z;
	for (j = 1; j <= dx; j++, x += sx)
	{ // первый "брезенхем", в счётном цикле
		if (d > 0)
		{
			d += d2;
			y += sy;

			// был выполнен шаг по средней оси - 'y'. 
			// второй "брезенхнм"
			if (dd > 0)
			{
				dd += dd2;
				z += sz;
			}
			else dd += dd1;
			// конец второго "брезенхема"

		}
		else d += d1;
		// конец первого "брезенхема"

		//SetPoint(buffer, x, y, color); // вывод очередной точки

	};

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

void SetLine(BitmapBuffer* buffer, std::vector<std::vector<double>> coordsharow, BitmapRGB color)
{
	int j = 0;
	for (int i = 0; i < coordsharow.size(); i++) {
		j = i + 1;
		if (j == coordsharow.size()) {
			j = 0;
		}
		double const deltaX = abs(coordsharow.at(i).at(0) - coordsharow.at(j).at(0));
		double const deltaY = abs(coordsharow.at(i).at(1) - coordsharow.at(j).at(1));
		double x_1, x_2, y_1, y_2;
		double x, y, xend, yend, s, d, inc1, inc2;
		
		x_1 = coordsharow.at(i).at(0);
		x_2 = coordsharow.at(j).at(0);
		y_1 = coordsharow.at(i).at(1);
		y_2 = coordsharow.at(j).at(1);
		
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
				SetPoint(buffer, x, y, color);
				while (x < xend) {
					x++;
					if (d > 0) {
						y += s;
						d += inc2;
					}
					else
						d += inc1;

					SetPoint(buffer, x, y, color);
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

				SetPoint(buffer, x, y, color);
				while (y < yend) {
					y++;
					if (d > 0) {
						x += s;
						d += inc2;
					}
					else
						d += inc1;

					SetPoint(buffer, x, y, color);
				}

			}
	}
	
}

void SetLine(BitmapBuffer* buffer, Earth* pointer, int apex, BitmapRGB color)
{
	int j = 0;
	for (int i = 0; i < apex; i++) {
		j = i + 1;
		if (j == apex) {
			j = 0;
		}
		double const deltaX = abs(pointer[i].coordEarth.x - pointer[j].coordEarth.x);
		double const deltaY = abs(pointer[i].coordEarth.y - pointer[j].coordEarth.y);
		double x_1, x_2, y_1, y_2;
		double x, y, xend, yend, s, d, inc1, inc2;

		x_1 = pointer[i].coordEarth.x;
		x_2 = pointer[j].coordEarth.x;
		y_1 = pointer[i].coordEarth.y;
		y_2 = pointer[j].coordEarth.y;
		
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
				///SetPoint(buffer, x, y, color);
				while (x < xend) {
					x++;
					if (d > 0) {
						y += s;
						d += inc2;
					}
					else
						d += inc1;

					//SetPoint(buffer, x, y, color);
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

				////SetPoint(buffer, x, y, color);
				while (y < yend) {
					y++;
					if (d > 0) {
						x += s;
						d += inc2;
					}
					else
						d += inc1;

					//SetPoint(buffer, x, y, color);
				}

			}

	}

}

/*
std::vector <double> operator-(const std::vector<double>& a, const std::vector<double>& b)
{
	vector<double> c(a.size());
	for (size_t i = 0; i < a.size(); ++i)
		c[i] = a[i] - b[i];
	return c;
}

*/

double operator*(std::vector<double>& a, std::vector<double>& b)
{ return a[0] * b[0] + a[1] * b[1]+a[2]*b[2]; }

double abs(const std::vector<double>& a) {
	double c = 0;
	double sum = 0;
	for (int i = 0; i < a.size(); i++)
		sum += pow(a[i], 2);
	c = sqrt(sum);
	/*
	if(a.size()==3)
		 c = std::sqrt(pow(a[0], 2) + pow(a[1], 2) + pow(a[2], 2));
	if(a.size()==2)
		c = std::sqrt(pow(a[0], 2) + pow(a[1], 2));
		*/
	return c;
}

std::vector <double> operator*(const std::vector<double>& a, double k) {
	std::vector<double> b = { a[0] * k, a[1] * k, a[2] * k };
	return b;
}

std::vector <double> operator/(const std::vector<double>& a, double k) {
	std::vector<double> b = { a[0] / k, a[1] / k, a[2] / k };
	return b;
}

std::vector <double> operator+(const std::vector<double>& a, const std::vector<double>& b) {
	if (a.size() != b.size())
		throw("a.size() != b.size()"); // Или как-то иначе обработать разные размеры
	vector<double> c(a.size());
	for (size_t i = 0; i < a.size(); ++i)
		c[i] = a[i] + b[i];
	return c;
}

std::vector <double> operator^(const std::vector<double>& a, const std::vector<double>& b) {
	std::vector<double> r = { a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0] };
	return r;
}

inline double get_signum(double val) {
	return val < 0 ? -1
		: val == 0 ? 0
		: 1;
}


void SetLine3D(BitmapBuffer* buffer, std::vector<std::vector<double>> coordsharow, int i, int i1, BitmapRGB color) {
	if (round(coordsharow[i][0]) != round(coordsharow[i1][0])) {
		vector <double> P1 = { coordsharow[i][0],  coordsharow[i][1],  coordsharow[i][2] };
		vector <double> P2 = { coordsharow[i1][0], coordsharow[i1][1], coordsharow[i1][2] };

		vector <double> D = { P2[0] - P1[0], P2[1] - P1[1], P2[2] - P1[2] };
		double d, x1, x2, y1, y2, z1, z2, dx, dy, dz, ax, ay, az, sx, sy, sz, x, y, z, ind, zd, yd, xd, idx;
		for (unsigned int i = 0; i < D.size(); i++)
		{
			if (D[i] < 0) {
				D[i] *= -1; //make positive.    _OR_   use D[i] = abs(D[i]);
				D[i]++;
			}
		}
		d = *std::max_element(D.begin(), D.end());
		std::vector <double>X;
		std::vector <double>Y;
		std::vector <double>Z;

		x1 = P1[0];
		y1 = P1[1];
		z1 = P1[2];

		x2 = P2[0];
		y2 = P2[1];
		z2 = P2[2];

		dx = x2 - x1;
		dy = y2 - y1;
		dz = z2 - z1;

		ax = abs(dx) * 2;
		ay = abs(dy) * 2;
		az = abs(dz) * 2;


		sx = dx > 0 ? 1 : -1;
		sy = dy > 0 ? 1 : -1;
		sz = dz > 0 ? 1 : -1;

		x = x1;
		y = y1;
		z = z1;
		idx = 0;

		if (ax >= max(ay, az)) {
			yd = ay - ax / 2;
			zd = az - ax / 2;

			while (1) {
				//to do трассировку в камеру


				X.push_back(x);
				Y.push_back(y);
				Z.push_back(z);
				idx = idx + 1;

				if (round(x) == round(x2)) {
					break;
				}
				if (yd >= 0) {
					y = y + sy;
					yd = yd - ax;
				}
				if (zd >= 0) {
					z = z + sz;
					zd = zd - ax;
				}
				x = x + sx;
				yd = yd + ay;
				zd = zd + az;
			}
		}
		else
		{
			if (ay >= max(ax, az))
			{
				xd = ax - ay / 2;
				zd = az - ay / 2;

				while (1) {
					//to do трассировку в камеру


					X.push_back(x);
					Y.push_back(y);
					Z.push_back(z);
					idx = idx + 1;

					if (round(y) == round(y2))
						break;

					if (xd >= 0) {
						x = x + sx;
						xd = xd - ay;
					}

					if (zd >= 0) {
						z = z + sz;
						zd = zd - ay;
					}
					y = y + sy;
					xd = xd + ax;
					zd = zd + az;
				}
			}
			else {
				if (az >= max(ax, ay)) {
					xd = ax - az / 2;
					yd = ay - az / 2;

					while (1) {
						//to do трассировку в камеру

						X.push_back(x);
						Y.push_back(y);
						Z.push_back(z);

						idx = idx + 1;

						if (round(z) == round(z2))
							break;

						if (xd >= 0) {
							x = x + sx;
							xd = xd - az;
						}

						if (yd >= 0) {
							y = y + sy;
							yd = yd - az;
						}
						z = z + sz;
						xd = xd + ax;
						yd = yd + ay;
					}
				}
			}

		}
		for (int i = 0; i < X.size(); i++) {
			SetPoint(buffer, X[i], Y[i], color);
		}
		X.clear();
		Y.clear();
		Z.clear();
		X.shrink_to_fit();
		Y.shrink_to_fit();
		Z.shrink_to_fit();
	}
}



void CentralProjection(Camera& camera, Apex& apex, PointBuffer& pointer) {
	//new matrix
	for (int i = apex.GetApex()/2; i < apex.GetApex(); i++) {
		const int N = 3; //колвo строк
		const int M = 1; //колво столбцов результата и еще одной матрицы
		double det = 0;
		double funcMatrix[N][N] = { {camera.coordCam.y - pointer[i - apex.GetApex()/2].y, pointer[i - apex.GetApex()/2].x - camera.coordCam.x , 0},
									{camera.coordCam.z - pointer[i - apex.GetApex()/2].z, 0, pointer[i - apex.GetApex()/2].x - camera.coordCam.x},
									{0, 0, 1} };

		double xyzMatrix[N][M] = { {((pointer[i - apex.GetApex()/2].x * (camera.coordCam.y - pointer[i - apex.GetApex()/2].y)) - (pointer[i - apex.GetApex()/2].y * (camera.coordCam.x - pointer[i - apex.GetApex()/2].x)))},
								   {((pointer[i - apex.GetApex()/2].x * (camera.coordCam.z - pointer[i - apex.GetApex()/2].z)) - (pointer[i - apex.GetApex()/2].z * (camera.coordCam.x - pointer[i - apex.GetApex()/2].x)))},
								   {0} };

		double product[N][M] = { {0},
								 {0},
								 {0}, };

		//создание двухмерного динамического массива
		double** ptrMatrix = dynamic_array_alloc(N, N);
		double** obt_Matrix = dynamic_array_alloc(N, N);
		double** tMatrix = dynamic_array_alloc(N, N);
		for (int count = 0; count < N; count++) {
			for (int j = 0; j < N; j++) {
				ptrMatrix[count][j] = funcMatrix[count][j];
			}
		}

		det = Determinant(ptrMatrix, N);

		if (det) {
			TransnMatrixx(ptrMatrix, obt_Matrix, N);
			for (int count_i = 0; count_i < N; count_i++) {
				for (int count_j = 0; count_j < N; count_j++) {
					int p = N - 1; //новый размер матрицы
					double** temp_matrix = dynamic_array_alloc(p, p);
					Get_matrix(obt_Matrix, N, temp_matrix, count_i, count_j);
					tMatrix[count_i][count_j] = pow(-1.0, count_i + count_j + 2) * Determinant(temp_matrix, p) / det;
					dynamic_array_free(temp_matrix, p);
				}
			}

			for (int row = 0; row < N; row++) {
				for (int col = 0; col < M; col++) {
					for (int inner = 0; inner < N; inner++) {
						product[row][col] += tMatrix[row][inner] * xyzMatrix[inner][col];
					}
				}
			}
			pointer[i].x = product[0][0];
			pointer[i].y = product[1][0];
			pointer[i].z = product[2][0];
		}
		dynamic_array_free(ptrMatrix, N);
		dynamic_array_free(obt_Matrix, N);
	}
}

//поиск детерминанта
double Determinant(double** T, int N)
{

	double det__;
	int sub_j, s;
	double** subT;    // Субматрица как набор ссылок на исходную матрицу
	switch (N)
	{
	case 1:
		return T[0][0];
	case 2: {
		double res = T[0][0] * T[1][1] - T[0][1] * T[1][0];
		return res;
	}
	default:

		if (N < 1) return nan("1");  // Некорректная матрица

		subT = new double* [N - 1];  // Массив ссылок на столбцы субматрицы
		det__ = 0;
		s = 1;        // Знак минора
		for (int i = 0; i < N; i++)  // Разложение по первому столбцу
		{
			sub_j = 0;
			for (int j = 0; j < N; j++) {// Заполнение субматрицы ссылками на исходные столбцы 
				if (i != j) {  // исключить i строку
					subT[sub_j++] = T[j] + 1;  // здесь + 1 исключает первый столбец

				}
			}
			det__ = det__ + s * T[i][0] * Determinant(subT, N - 1);
			s = -s;
		}
		delete[] subT;
		return det__;
	};
};

double** dynamic_array_alloc(size_t N, size_t M) {
	double** A = (double**)malloc(N * sizeof(double*));
	for (int i = 0; i < N; i++)
		A[i] = (double*)malloc(M * sizeof(double));
	return A;
}

void dynamic_array_free(double** A, size_t N) {
	for (int i = 0; i < N; i++)
		free(A[i]);
	free(A);
}

void Get_matrix(double** matrix, int n, double** temp_matrix, int indRow, int indCol) {
	int ki = 0;
	for (int i = 0; i < n; i++) {
		if (i != indRow) {
			for (int j = 0, kj = 0; j < n; j++) {
				if (j != indCol) {
					temp_matrix[ki][kj] = matrix[i][j];
					kj++;
				}
			}
			ki++;
		}
	}
}

//транспонирование матрицы
void TransnMatrixx(double** matrix, double** tMatrix, int n) {
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			tMatrix[j][i] = matrix[i][j];
}

bool proverka(BitmapBuffer* buffer, int x, int y, BitmapRGB color) {
	if ((y < static_cast<int>(buffer->GetHeight())) &&
		(y > 0) &&
		(x < static_cast<int>(buffer->GetWidth())) &&
		(x > 0)) {
		if ((((*buffer)[y][x].red == color.red) &&
			((*buffer)[y][x].green == color.green) &&
			((*buffer)[y][x].blue == color.blue))) {
			return 1;
		}
	}
	return 0;
}

void visovSketch(BitmapBuffer* buffer, int* xy, PointBuffer& point, BitmapRGB OldColor, BitmapRGB NewColor) {
	int cnt[4];
	int vse = 0;
	for (int i = 0; i < 4; i++) {
		cnt[i] = 0;
	}
	for (int i = 0; i < 4; i++) {
		if (proverka(buffer, point[xy[i]].x, point[xy[i]].y, OldColor)) {
			cnt[i]++;
		}
	}
	for (int i = 0; i < 4; i++) {
		vse +=cnt[i];
	}
	double Dot[2];
	if (vse == 0 && cross(point,Dot,xy)) {
		
		//Sketch(buffer, Dot[0], Dot[1], point, OldColor, NewColor);
	}
}
/*
void visovTeni(BitmapBuffer* buffer, PointBuffer& pointer, Camera& cam, Earth* earth, Ten*many) {
	int ind[2][3] = { {0, 1, 2}, {3, 1, 2} };
	std::vector<double> O;
	std::vector<double> Y = { cam.coordLight.x, cam.coordLight.y, cam.coordLight.z };
	for (int i = 0; i < 8; i++) {
		if (!proverka(buffer, pointer[i].x, pointer[i].y, { 255,255,255 })) {
			for (int j = 0; j < 2; j++) {
				std::vector<double> X = { pointer[i].x, pointer[i].y, pointer[i].z };
				if (IntersectionWithEart(earth, (int*)ind[j], X, Y, O)) {
					many[i].coordTen.x = O[0];
					many[i].coordTen.y = O[1];
					many[i].coordTen.z = O[2];
					O.clear();
				}
			}
			
		}


	}
}
*/
/*
void Sketch(BitmapBuffer* buffer, double x, double y, PointBuffer& point, BitmapRGB OldColor, BitmapRGB NewColor)
{
	std::vector<double> a;
	std::vector<double> b;
	std::vector<double> c;
	std::vector<double> p;
	p.push_back(x);
	p.push_back(y);
	int ind[12][3] = { {3,0,1},
		{3,2,1},
		{0,3,7},
		{0,4,7},
		{0,4,5},
		{0,1,5},
		{7,6,5},
		{7,4,5},
		{1,2,6},
		{1,6,5},
		{3,7,6},
		{3,2,6} };
	int cnt = 0;
	for (int i = 0; i < 12; i++) {
		
		
		a.push_back(point[ind[i][0]].x);
		a.push_back(point[ind[i][0]].y);
		
		b.push_back(point[ind[i][1]].x);
		b.push_back(point[ind[i][1]].y);
		
		c.push_back(point[ind[i][2]].x);
		c.push_back(point[ind[i][2]].y);
		if (pointInTriangle(p,a,b,c)) cnt++;
		a.clear();
		b.clear();
		c.clear();
	}
	if ((cnt==3)&& (y < static_cast<int>(buffer->GetHeight())) &&
		(y > 0) && 
		(x < static_cast<int>(buffer->GetWidth())) && 
		(x > 0))
	{
		int x1 = x;
		int y1 = y;
		if (((*buffer)[y1][x1].red == OldColor.red) &&
			((*buffer)[y1][x1].green == OldColor.green) &&
			((*buffer)[y1][x1].blue == OldColor.blue))
		{
			SetPoint(buffer, x, y,  NewColor);
			Sketch(buffer, x + 1, y,  point, OldColor, NewColor);
			Sketch(buffer, x - 1, y,  point, OldColor, NewColor);
			Sketch(buffer, x, y - 1,  point, OldColor, NewColor);
			Sketch(buffer, x, y + 1,  point, OldColor, NewColor);
		}
	}
}

*/

bool pointInPolygon(PointBuffer& point, int x, int y)
{

	int   i, j = 4 - 1;
	bool  oddNodes = 0;

	for (i = 0; i < 4; i++) {
		if ((point[i].y < y && point[j].y >= y
			|| point[j].y < y && point[i].y >= y)
			&& (point[i].x <= x || point[j].x <= x)) {
			oddNodes ^= (point[i].x + (y - point[i].y) / (point[j].y - point[i].y) * (point[j].x - point[i].x) < x);
		}
		j = i;
	}

	return oddNodes;
}

/**



// Заливает строку iy от ixn до ixk

void FILSTR(RGBQUAD* buffer, uint32_t w, int iy, int ixn, int ixk, RGBQUAD color) {
	while (ixn <= ixk) SetPoint(buffer, ixn++, iy, w, color);
} // FILSTR

static double* pt;

static int NextApex;	//Номер следующей вершины в списке 
double* yUP = new double[point.SetApex()];	//Y-коориданты вершин по возрастанию 
int* yNumb = new int[point.SetApex()]; // их номера 

//Список активных ребер
static int IDLSPI;			//Длина списка активных ребер
static int IYREB[300];		//Макс Y-коорд активных ребер
static double RXREB[300];	//Тек X-коорд акивных ребер
static double RPRIP[300];	//X-приращение на 1 шаг по Y
static double RYSL[300];	//Dy между тек и соседн верш 
							//Dy <= 0.0 - обычная вершина 
							//	  > 0.0 - локал эксремум 

static int FORSPI(RGBQUAD* buffer, uint32_t w, int IYBEG, RGBQUAD color) {
	int i, ikledg, intek, intabs, isd;
	int iyt, ixt, nrebra, inc, inpred, inposl;
	double xt, xc, yt, yc, dy;

	ikledg = 0;
	for (i = NextApex; i < point.SetApex(); ++i)
		if (yUP[i] != IYBEG) break; else ikledg++;

	for (i = 0; i < ikledg; i++) {
		intek = yNumb[NextApex + i];
		intabs = abs(intek);
		xt = pt[intabs - 1];
		yt = pt[intabs];

		if ((inpred = intabs - 2) < 0) inpred = yNumb[(point.SetApex() - 1)]; //правка - APEX*2-1
		if ((inposl = intabs + 2) > (point.SetApex() * 2 - 1)) inposl = yNumb[0]; //правка - 1

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
	if ((i = (NextApex += ikledg)) > (point.SetApex() - 1)) i = (point.SetApex() - 1);
	return(yUP[i]);
}

//
// Однотонно заливает многоугольник,
//заданный координатами вершин

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
	//Построение массивов У и их номеров
	int cnt = 0;
	for (i = 1; i < (point.SetApex() * 2); i += 2) {
		yUP[cnt] = *(pt + i);
		yNumb[cnt] = i;
		cnt++;
	}
	//Сортировка

	bool sort_or_not = 1;

	do {
		sort_or_not = 1;
		for (int i = 0; i < (point.SetApex() - 1); i++) { // n - размер сортируемого массива
			if (yUP[i] > yUP[i + 1]) {
				swaping(yUP[i], yUP[i + 1]);
				swaping_int(yNumb[i], yNumb[i + 1]);
				sort_or_not = 1;
			}
		}
		for (int i = (point.SetApex() - 1); i >= 1; i--) {
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
	
	//Начальные присвоения
	IDLSPI = 0;
	NextApex = 0;
	iybeg = yUP[0];
	iymak = yUP[point.SetApex() - 1];

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
*/


void Carryover(Apex& apex, PointBuffer& pointer, char ind, double angle) {
	double Dot[2];
	double Dx1, Dy1;
	int a[4] = { 0,
		1,
		6,
		7};
	if (cross(pointer, Dot, a)) {
		Dx1 = Dot[0];
		Dy1 = Dot[1];
		for (int i = 0; i < apex.GetApex(); i++) {
			const int N = 4; //колвo строк
			const int M = 1; //колво столбцов результата
			double preCoord[N][M] = { {pointer[i].x},
										{pointer[i].y}, 
										{pointer[i].z}, 
										{1} };
			double CarryMatr[N][N] = { {1, 0 , 0, 0},
										{0, 1 , 0, 0},
										{0, 0 , 1, 0},
										{-Dx1, -Dy1, 0, 1} };
			double Prod[N][M] = { {0},
									{0},
									{0},
									{0} };
			for (int row = 0; row < N; row++) {
				for (int col = 0; col < M; col++) {
					for (int inner = 0; inner < N; inner++) {
						Prod[row][col] += CarryMatr[inner][row] * preCoord[inner][col];
					}
				}
			}
			pointer[i].x = Prod[0][0];
			pointer[i].y = Prod[1][0];
			pointer[i].z = Prod[2][0];
		}
	}
	
	switch (ind)
	{
	case 'y':
		//Ry(apex, pointer, angle);
		break;
	case 'x':
		//Rx(apex, pointer, angle);
		break;
	case 'z':
		//Rz(apex, pointer, angle);
		break;
	}
	
	for (int i = 0; i < apex.GetApex() ; i++) {
		const int N = 4; //колвo строк
		const int M = 1; //колво столбцов результата
		double preCoord[N][M] = { {pointer[i].x},
									{pointer[i].y},
									{pointer[i].z},
									{1} };
		double CarryMatr[N][N] = { {1, 0 , 0, 0},
									{0, 1 , 0, 0},
									{0, 0 , 1, 0},
									{Dx1, Dy1, 0, 1} };
		double Prod[N][M] = { {0},
								{0},
								{0},
								{0} };
		for (int row = 0; row < N; row++) {
			for (int col = 0; col < M; col++) {
				for (int inner = 0; inner < N; inner++) {
					Prod[row][col] += CarryMatr[inner][row] * preCoord[inner][col];
				}
			}
		}
		pointer[i].x = Prod[0][0];
		pointer[i].y = Prod[1][0];
		pointer[i].z = Prod[2][0];
	}
	
	
}

bool cross(PointBuffer& pointer, double* dot, int* ind) {
	double n;
	if (pointer[ind[2]].y - pointer[ind[0]].y != 0) {  // a(y)
		double q = (pointer[ind[2]].x - pointer[ind[0]].x) / (pointer[ind[0]].y - pointer[ind[2]].y);
		double sn = (pointer[ind[1]].x - pointer[ind[3]].x) + (pointer[ind[1]].y - pointer[ind[3]].y) * q; if (!sn) { return 0; }  // c(x) + c(y)*q
		double fn = (pointer[ind[1]].x - pointer[ind[0]].x) + (pointer[ind[1]].y - pointer[ind[0]].y) * q;   // b(x) + b(y)*q
		n = fn / sn;
	}
	else {
		if (!(pointer[ind[1]].y - pointer[ind[3]].y)) { return 0; }  // b(y)
		n = (pointer[ind[1]].y - pointer[ind[0]].y) / (pointer[ind[1]].y - pointer[ind[3]].y);   // c(y)/b(y)
	}
	dot[0] = pointer[ind[1]].x + (pointer[ind[3]].x - pointer[ind[1]].x) * n;  // x3 + (-b(x))*n
	dot[1] = pointer[ind[1]].y + (pointer[ind[3]].y - pointer[ind[1]].y) * n;  // y3 +(-b(y))*n
	return 1;
}



void Rz(Apex& apex, PointBuffer& pointer, double angle) //поворот вокруг оси Z
{
	const int N = 4; //колвo строк
	const int M = 1; //колво столбцов результата
	for (int i = 0; i < apex.GetApex(); i++) {
		double preCoord[N][M] = { {pointer[i].x},
										{pointer[i].y},
										{pointer[i].z},
										{1} };
		double CarryMatr[N][N] = { {cos(angle*PI/180), sin(angle * PI / 180) , 0, 0},
									{-sin(angle * PI / 180), cos(angle * PI / 180) , 0, 0},
									{0, 0 , 1, 0},
									{0, 0, 0, 1} };
		double Prod[N][M] = { {0},
								{0},
								{0},
								{0} };
		for (int row = 0; row < N; row++) {
			for (int col = 0; col < M; col++) {
				for (int inner = 0; inner < N; inner++) {
					Prod[row][col] += CarryMatr[inner][row] * preCoord[inner][col];
				}
			}
		}
		pointer[i].x = Prod[0][0];
		pointer[i].y = Prod[1][0];
		pointer[i].z = Prod[2][0];
	}
}

void Rz(Apex& apex, Ten* pointer, double angle) //поворот вокруг оси Z
{
	const int N = 4; //колвo строк
	const int M = 1; //колво столбцов результата
	for (int i = 0; i < apex.GetApex(); i++) {
		double preCoord[N][M] = { {pointer[i].coordTen.x},
										{pointer[i].coordTen.y},
										{pointer[i].coordTen.z},
										{1} };
		double CarryMatr[N][N] = { {cos(angle * PI / 180), sin(angle * PI / 180) , 0, 0},
									{-sin(angle * PI / 180), cos(angle * PI / 180) , 0, 0},
									{0, 0 , 1, 0},
									{0, 0, 0, 1} };
		double Prod[N][M] = { {0},
								{0},
								{0},
								{0} };
		for (int row = 0; row < N; row++) {
			for (int col = 0; col < M; col++) {
				for (int inner = 0; inner < N; inner++) {
					Prod[row][col] += CarryMatr[inner][row] * preCoord[inner][col];
				}
			}
		}
		pointer[i].coordTen.x = Prod[0][0];
		pointer[i].coordTen.y = Prod[1][0];
		pointer[i].coordTen.z = Prod[2][0];
	}
}

void Rx(Apex& apex, Ten* pointer, double angle) //поворот вокруг оси X
{
	const int N = 4; //колвo строк
	const int M = 1; //колво столбцов результата
	for (int i = 0; i < apex.GetApex(); i++) {
		double preCoord[N][M] = { {pointer[i].coordTen.x},
										{pointer[i].coordTen.y},
										{pointer[i].coordTen.z},
										{1} };
		double CarryMatr[N][N] = { {1, 0, 0, 0},
									{0, cos(angle * PI / 180) , -sin(angle * PI / 180), 0},
									{0, sin(angle * PI / 180) , cos(angle * PI / 180), 0},
									{0, 0, 0, 1} };
		double Prod[N][M] = { {0},
								{0},
								{0},
								{0} };
		for (int row = 0; row < N; row++) {
			for (int col = 0; col < M; col++) {
				for (int inner = 0; inner < N; inner++) {
					Prod[row][col] += CarryMatr[inner][row] * preCoord[inner][col];
				}
			}
		}
		pointer[i].coordTen.x = Prod[0][0];
		pointer[i].coordTen.y = Prod[1][0];
		pointer[i].coordTen.z = Prod[2][0];
	}
}

void Rx(Apex& apex, PointBuffer& pointer, double angle) //поворот вокруг оси X
{
	const int N = 4; //колвo строк
	const int M = 1; //колво столбцов результата
	for (int i = 0; i < apex.GetApex(); i++) {
		double preCoord[N][M] = { {pointer[i].x},
										{pointer[i].y},
										{pointer[i].z},
										{1} };
		double CarryMatr[N][N] = { {1, 0, 0, 0},
									{0, cos(angle * PI / 180) , -sin(angle * PI / 180), 0},
									{0, sin(angle * PI / 180) , cos(angle * PI / 180), 0},
									{0, 0, 0, 1} };
		double Prod[N][M] = { {0},
								{0},
								{0},
								{0} };
		for (int row = 0; row < N; row++) {
			for (int col = 0; col < M; col++) {
				for (int inner = 0; inner < N; inner++) {
					Prod[row][col] += CarryMatr[inner][row] * preCoord[inner][col];
				}
			}
		}
		pointer[i].x = Prod[0][0];
		pointer[i].y = Prod[1][0];
		pointer[i].z = Prod[2][0];
	}
}

void Ry(Apex& apex, PointBuffer& pointer, double angle) //поворот вокруг оси Y
{
	const int N = 4; //колвo строк
	const int M = 1; //колво столбцов результата
	for (int i = 0; i < apex.GetApex() ; i++) {
		double preCoord[N][M] = { {pointer[i].x},
										{pointer[i].y},
										{pointer[i].z},
										{1} };
		double CarryMatr[N][N] = { {cos(angle * PI / 180), 0, sin(angle * PI / 180), 0},
									{0, 1 , 0, 0},
									{-sin(angle * PI / 180), 0 , cos(angle * PI / 180), 0},
									{0, 0, 0, 1} };
		double Prod[N][M] = { {0},
								{0},
								{0},
								{0} };
		for (int row = 0; row < N; row++) {
			for (int col = 0; col < M; col++) {
				for (int inner = 0; inner < N; inner++) {
					Prod[row][col] += CarryMatr[inner][row] * preCoord[inner][col];
				}
			}
		}
		pointer[i].x = Prod[0][0];
		pointer[i].y = Prod[1][0];
		pointer[i].z = Prod[2][0];
	}
}

/*
int classify(const std::vector<double>& p0, const std::vector<double>& p1, const std::vector<double>& p2)
{
	std::vector<double> a = p1 - p0;
	std::vector<double> b = p2 - p0;
	double sa = a[0] * b[1] - b[0] * a[1];
	if (sa > 0.0)
		return LEFT;
	if (sa < 0.0)
		return RIGHT;
	if ((a[0] * b[0] < 0.0) || (a[1] * b[1] < 0.0))
		return BEHIND;
	if (abs(a) < abs(b))
		return BEYOND;
	if ((p0[0] == p2[0]) && (p0[1] == p2[1]))
		return ORIGIN;
	if ((p1[0] == p2[0]) && (p1[1] == p2[1]))
		return DESTINATION;
	return BETWEEN;
}

bool pointInTriangle(const std::vector<double>& p, const std::vector<double>& a, const std::vector<double>& b, const std::vector<double>& c)
{
	return ((classify(a, b, p) != LEFT) && (classify(b, c, p) != LEFT) && (classify(c, a, p) != LEFT));
}
*/