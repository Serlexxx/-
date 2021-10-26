#include "Painter.h"
using namespace std;



/**
* \brief ��������� �������
* \param buffer ����� ����� (��������� �� ������)
* \param x ��������� �� ��� X
* \param y ��������� �� ��� Y
* \param w ������ �����-������
* \param color ������� �����
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
 * \brief ��������� �����
 * \param width ������
 * \param height ������
 * \param pixels ������ ��������
 * \param hWnd ����� ����, device context �������� ����� �����������
 */
void PresentFrame( uint32_t width, uint32_t height, void* pixels, HWND hWnd)
{
	// �������� ����� �� ��������� bit-map (4 ����� �� �������)
	HBITMAP hBitMap = CreateBitmap(width, height, 1, 8 * 4, pixels);

	// �������� device context ����
	HDC hdc = GetDC(hWnd);

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
		SRCCOPY // ���������� //���� SRCCOPY, ������� �� SRCAND
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
void SetLine(BitmapBuffer* buffer, Point* pointer, Apex&apex, Camera& camera, BitmapRGB color)
{
	int cnt = 0;
	int j = 0;
	for (int i = 0; i < apex.GetApex(); i++) {

		if (i < (apex.GetApex() - (apex.GetApex()/2)-1)) {
			j++;
			cnt++;
		}
		else {
			j = 0;
		}
		if (i>=apex.GetApex()/2) {
			if (i < apex.GetApex() - 1) {
				j = i + 1;
			}
			else {
				j = apex.GetApex() / 2;
			}	
		}

		double const deltaX = abs(pointer[i].GetPoint('x') - pointer[j].GetPoint('x'));
		double const deltaY = abs(pointer[i].GetPoint('y') - pointer[j].GetPoint('y'));
		double x_1, x_2, y_1, y_2;
		double x, y, xend, yend, s, d, inc1, inc2;
		
		x_1 = pointer[i].GetPoint('x');
		x_2 = pointer[j].GetPoint('x');
		y_1 = pointer[i].GetPoint('y');
		y_2 = pointer[j].GetPoint('y');

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
	for (int i = 0; i < apex.GetApex()/2; i++) {
		j = i + apex.GetApex() / 2;

		double const deltaX = abs(pointer[i].GetPoint('x') - pointer[j].GetPoint('x'));
		double const deltaY = abs(pointer[i].GetPoint('y') - pointer[j].GetPoint('y'));
		double x_1, x_2, y_1, y_2;
		double x, y, xend, yend, s, d, inc1, inc2;
		x_1 = pointer[i].GetPoint('x');
		x_2 = pointer[j].GetPoint('x');
		y_1 = pointer[i].GetPoint('y');
		y_2 = pointer[j].GetPoint('y');

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
	CentralProjection(camera, apex, pointer);
}




void CentralProjection(Camera& camera, Apex& apex, Point* pointer) {
	//new matrix
	for (int i = 4; i < apex.GetApex(); i++) {
		const int N = 3; //����� �����
		const int M = 1; //����� �������� ���������� � ��� ����� �������
		double det = 0;
		double funcMatrix[N][N] = { {camera.coordCam.y - pointer[i - 4].GetPoint('y'), pointer[i - 4].GetPoint('x') - camera.coordCam.x , 0},
									{camera.coordCam.z - pointer[i - 4].GetPoint('z'), 0, pointer[i - 4].GetPoint('x') - camera.coordCam.x},
									{0, 0, 1} };

		double xyzMatrix[N][M] = { {((pointer[i - 4].GetPoint('x') * (camera.coordCam.y - pointer[i - 4].GetPoint('y'))) - (pointer[i - 4].GetPoint('y') * (camera.coordCam.x - pointer[i - 4].GetPoint('x'))))},
								   {((pointer[i - 4].GetPoint('x') * (camera.coordCam.z - pointer[i - 4].GetPoint('z'))) - (pointer[i - 4].GetPoint('z') * (camera.coordCam.x - pointer[i - 4].GetPoint('x'))))},
								   {0} };

		double product[N][M] = { {0},
								 {0},
								 {0}, };

		//�������� ����������� ������������� �������
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
					int p = N - 1; //����� ������ �������
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
			pointer[i].SetPoint('x', product[0][0]);
			pointer[i].SetPoint('y', product[1][0]);
			pointer[i].SetPoint('z', product[2][0]);
		}
		dynamic_array_free(ptrMatrix, N);
		dynamic_array_free(obt_Matrix, N);
	}
}


double Determinant(double** T, int N)
{

	double det__;
	int sub_j, s;
	double** subT;    // ���������� ��� ����� ������ �� �������� �������
	switch (N)
	{
	case 1:
		return T[0][0];
	case 2: {
		double res = T[0][0] * T[1][1] - T[0][1] * T[1][0];
		return res;
	}
	default:

		if (N < 1) return nan("1");  // ������������ �������

		subT = new double* [N - 1];  // ������ ������ �� ������� ����������
		det__ = 0;
		s = 1;        // ���� ������
		for (int i = 0; i < N; i++)  // ���������� �� ������� �������
		{
			sub_j = 0;
			for (int j = 0; j < N; j++) {// ���������� ���������� �������� �� �������� ������� 
				if (i != j) {  // ��������� i ������
					subT[sub_j++] = T[j] + 1;  // ����� + 1 ��������� ������ �������

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

void TransnMatrixx(double** matrix, double** tMatrix, int n) {
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			tMatrix[j][i] = matrix[i][j];
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

