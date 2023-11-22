#include "Painter.h"
using namespace std;
#define PI 3.14159265


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
/*
* ������� ��������� ��� 3D
*/
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
				//to do ����������� � ������


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
					//to do ����������� � ������


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
						//to do ����������� � ������

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