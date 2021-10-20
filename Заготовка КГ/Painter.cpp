#include "Painter.h"
using namespace std;

/**
* \brief �������� ������ ����� (��������� ������ �������� RGBQUAD)
* \param width ������ ������ �����
* \param height ������ ������ �����
* \param clearColor ����������� ����
* \return ��������� �� ������
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
* \brief ���������� ������ ����������� �����-�� ���������� ������
* \param buffer ����� ����� (��������� �� ������)
* \param pixelCount ���-�� �������� � ������
* \param clearColor ����
*/
void ClearFrame(RGBQUAD* buffer, uint32_t pixelCount, RGBQUAD clearColor)
{
	fill_n(buffer, pixelCount, clearColor);
}

/**
* \brief ��������� �������
* \param buffer ����� ����� (��������� �� ������)
* \param x ��������� �� ��� X
* \param y ��������� �� ��� Y
* \param w ������ �����-������
* \param color ������� �����
*/
void SetPoint(RGBQUAD* buffer, int x, int y, uint32_t w, RGBQUAD color)
{

	buffer[(y * w) + x] = color;
}

/**
 * \brief ��������� �����
 * \param width ������
 * \param height ������
 * \param pixels ������ ��������
 * \param hWnd ����� ����, device context �������� ����� �����������
 */
void PresentFrame(HDC hdc, uint32_t width, uint32_t height, void* pixels, HWND hWnd)
{
	// �������� ����� �� ��������� bit-map (4 ����� �� �������)
	HBITMAP hBitMap = CreateBitmap(width, height, 1, 8 * 4, pixels);

	// ��������� DC ��� �������� bit-map'�
	HDC srcHdc = CreateCompatibleDC(hdc);

	// ������� bit-map � ��������� DC
	SelectObject(srcHdc, hBitMap);

	// ���������� ���������� ���������� DC � DC ����
	BitBlt(
		hdc,    // HDC ����������
		0,      // ������ ������� �� ��� X
		0,      // ������ ������� �� ��� Y
		width,  // ������
		height, // ������
		srcHdc, // �������� HDC (�� �������� ����� ������������ ������)
		0,      // ������ ���������� �� ��� X
		0,      // ������ ���������� �� ��� Y
		SRCCOPY // ����������
	);
	//Sleep(10000);
	// ���������� bit-map
	DeleteObject(hBitMap);
	// ���������� ��������� DC
	DeleteDC(srcHdc);
	// ���������� DC
	DeleteDC(hdc);
	ReleaseDC(hWnd, hdc);
	//PostQuitMessage(0);
}




/**
* \brief ��������� ����� (������� �������, �������� ����������)
* \param buffer ����� ����� (��������� �� ������)
* \param x0 ��������� ����� (���������� X)
* \param y0 ��������� ����� (���������� Y)
* \param x1 �������� ����� (���������� X)
* \param y1 �������� ����� (���������� Y)
* \param w ������ �����-������
* \param color ������� �����
*/
void SetLine(RGBQUAD* buffer, uint32_t w, Point& point, RGBQUAD color)
{
	int j = 0;
	for (int i = 0; i < point.SetApex(); i++) {
		i < (point.SetApex() - 1) ? j++ : j = 0;
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

//to do:: ���������� ���������� ����� � ���� setline
/**
* \brief ��������� ����� (������� �������, �������� ����������)
* \param buffer ����� ����� (��������� �� ������)
* \param x0 ��������� ����� (���������� X)
* \param y0 ��������� ����� (���������� Y)
* \param x1 �������� ����� (���������� X)
* \param y1 �������� ����� (���������� Y)
* \param w ������ �����-������
* \param color ������� �����
*/
void SetLine_pr(RGBQUAD* buffer, uint32_t w, Point& point, RGBQUAD color)
{
	int j = 0;
	for (int i = 0; i < point.SetApex(); i++) {
		i < (point.SetApex() - 1) ? j++ : j = 0;
		double const deltaX = abs(point.pointer_pr[i].x - point.pointer_pr[j].x);
		double const deltaY = abs(point.pointer_pr[i].y - point.pointer_pr[j].y);
		double x_1, x_2, y_1, y_2;
		double x, y, xend, yend, s, d, inc1, inc2;
		double check_x, check_y;
		x_1 = point.pointer_pr[i].x;
		x_2 = point.pointer_pr[j].x;
		y_1 = point.pointer_pr[i].y;
		y_2 = point.pointer_pr[j].y;

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
* \param k  ����������� ��������������� �����
* \param height ������ ������� �������
* \param width ������ ������� �������
* */
void MatrixMultiplication(double k, double height, double width, Point & point)
{
	int check = 0;
	struct Prod
	{
		double x, y;
	};

	Prod* ResProd = new Prod[point.SetApex()];
	int diag = 0;
	double cnt_apex = point.SetApex();
	double Dx, Dy;

	if (point.SetApex() % 2 == 1) {
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
		Dx = (point.pointer[0].x + point.pointer[point.SetApex() / 2].x) / 2;
		Dy = (point.pointer[0].y + point.pointer[point.SetApex() / 2].y) / 2;
	}
	for (int cnt = 0; cnt < point.SetApex(); cnt++) {


		double aMatrix[1][3] = { {point.pointer[cnt].x, point.pointer[cnt].y, 1} };
		double bMatrix[3][3] = { {k, 0, 0}, {0, k, 0}, {0, 0, 1} };
		double product[1][3] = { {0, 0, 0} };
		for (int i = 0; i < 1; i++) {
			for (int j = 0; j < 3; j++) {
				for (int inner = 0; inner < 3; inner++) {
					product[i][j] += floor(aMatrix[i][inner] * bMatrix[inner][j]);
				}
				//cout << product[i][j] << "  ";
			}
			//cout << endl;
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
			cout << ResProd[i].x << " x " << ResProd[i].y << endl;
			pointer[i].x = ResProd[i].x;
			pointer[i].y = ResProd[i].y;
		}
	}
	else {
		cout << "EROR: Border Reached" << endl;
		for (int i = 0; i < APEX; i++) {
			cout << ResProd[i].x << " x " << ResProd[i].y << endl;
		}
		cout << endl;
	}
	*/
	}
	for (int i = 0; i < point.SetApex(); i++) {
		cout << ResProd[i].x << " x " << ResProd[i].y << endl;
		point.pointer[i].x = ResProd[i].x;
		point.pointer[i].y = ResProd[i].y;
	}
}

/**
* \param angle  ����������� �������� �����
* \param height ������ ������� �������
* \param width ������ ������� �������
* */
void Rotate(double angle, double height, double width, Point &point)
{
	int cnt_apex = point.SetApex();
	int diag = 0;
	double Dx, Dy;

	if (point.SetApex() % 2 == 1) {
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
		Dx = (point.pointer[0].x + point.pointer[point.SetApex() / 2].x) / 2;
		Dy = (point.pointer[0].y + point.pointer[point.SetApex() / 2].y) / 2;
	}

	struct RadPoints
	{
		double xR, yR;
	};
	RadPoints *massiv = new RadPoints[point.SetApex()];
	int check = 0;

	for (int i = 0; i < point.SetApex(); i++) {
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
		cout << "EROR: Border Reached" << endl;
	}*/
}

/**



// �������� ������ iy �� ixn �� ixk

void FILSTR(RGBQUAD* buffer, uint32_t w, int iy, int ixn, int ixk, RGBQUAD color) {
	while (ixn <= ixk) SetPoint(buffer, ixn++, iy, w, color);
} // FILSTR

static double* pt;

static int NextApex;	//����� ��������� ������� � ������ 
double* yUP = new double[point.SetApex()];	//Y-���������� ������ �� ����������� 
int* yNumb = new int[point.SetApex()]; // �� ������ 

//������ �������� �����
static int IDLSPI;			//����� ������ �������� �����
static int IYREB[300];		//���� Y-����� �������� �����
static double RXREB[300];	//��� X-����� ������� �����
static double RPRIP[300];	//X-���������� �� 1 ��� �� Y
static double RYSL[300];	//Dy ����� ��� � ������ ���� 
							//Dy <= 0.0 - ������� ������� 
							//	  > 0.0 - ����� �������� 

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

		if ((inpred = intabs - 2) < 0) inpred = yNumb[(point.SetApex() - 1)]; //������ - APEX*2-1
		if ((inposl = intabs + 2) > (point.SetApex() * 2 - 1)) inposl = yNumb[0]; //������ - 1

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
// ��������� �������� �������������,
//�������� ������������ ������

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
	//���������� �������� � � �� �������
	int cnt = 0;
	for (i = 1; i < (point.SetApex() * 2); i += 2) {
		yUP[cnt] = *(pt + i);
		yNumb[cnt] = i;
		cnt++;
	}
	//����������

	bool sort_or_not = 1;

	do {
		sort_or_not = 1;
		for (int i = 0; i < (point.SetApex() - 1); i++) { // n - ������ ������������ �������
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
	
	//��������� ����������
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

