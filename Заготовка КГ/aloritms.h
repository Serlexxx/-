#pragma once
#define DEBUG true
#define M_PI 3.14159265358979323846
//#define PI 3.141592653589793238462643383279502884197169399375105820974944592307816
#define EPS 1.0E-9
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include "data.h"
#include "Camera.h"
#include "Earth.h"
#include "Painter.h"
enum { LEFT, RIGHT, BEYOND, BEHIND, BETWEEN, ORIGIN, DESTINATION };
//    СЛЕВА, СПРАВА, ВПЕРЕДИ, ПОЗАДИ, МЕЖДУ,   НАЧАЛО, КОНЕЦ
enum { INSIDE, OUTSIDE, BOUNDARY };         // положение точки
											//     ВНУТРИ, ВНЕ,     НА ГРАНИЦЕ
enum { TOUCHING, CROSSING, INESSENTIAL };   // положение ребра
											//     КАСАТЕЛbНОЕ, ПЕРЕСЕКАЮЩЕЕ, НЕСУЩЕСТВЕННОЕ
											
// класс некоторых действий линейной алгебры
template<class Type> class algorithm : public Data<Type> {
public: // открытые свойства и методы
    algorithm(uint ms, uint se) : Data<Type>(ms, se) {} // конструктор

    void bigelem() { // формирование системы с ведущим элементом
        uint k, i, j, kmax;
        Type maxval;
        for (k = 0; k < Data<Type>::sizes(); k++) {
            maxval = Data<Type>::readA(k, k);
            //  matA.at(k).at(k); // проход по диагонали
            kmax = k;
            for (j = k + 1; j < Data<Type>::sizes(); j++) { // больший элемент в столбце
                if (abs(Data<Type>::readA(j, k)) > abs(maxval)) {/*matA.at(j).at(k)*/
                    maxval = Data<Type>::readA(j, k);//matA.at(j).at(k);
                    kmax = j;
                }
            }
            if (kmax != k) { // если требуется перестановка
                for (i = 0; i < Data<Type>::sizes(); i++) { // перестановка строк матрицы A
                    maxval = Data<Type>::readA(k, i); // matA.at(k).at(i)
                    Data<Type>::savA(k, i, Data<Type>::readA(kmax, i)); // matA.at(k).at(i) = matA.at(kmax).at(i)
                    Data<Type>::savA(kmax, i, maxval); // matA.at(kmax).at(i) = maxval
                }
                maxval = Data<Type>::readB(k);// vecB.at(k); // перестановка элементов вектора B
                Data<Type>::savB(k, Data<Type>::readB(kmax)); // vecB.at(k) = vecB.at(kmax)
                Data<Type>::savB(kmax, maxval); // vecB.at(kmax) = maxval
            }
        }
    }

    void gauss() {  // метод исключения Гаусса
        int k, i, j;
        Type temp;
        for (k = 1; k < Data<Type>::sizes(); k++) { // прямой ход
            for (i = k; i < Data<Type>::sizes(); i++) {
                temp = Data<Type>::matA.at(i).at(k - 1) / Data<Type>::matA.at(k - 1).at(k - 1);
                for (j = 0; j < Data<Type>::sizes(); j++) {
                    Data<Type>::matA.at(i).at(j) = Data<Type>::matA.at(i).at(j) - temp * Data<Type>::matA.at(k - 1).at(j);
                }
                Data<Type>::vecB.at(i) = Data<Type>::vecB.at(i) - temp * Data<Type>::vecB.at(k - 1);
            }
            if (DEBUG == true) {
                cout << "Forward stroke, cycle: " << k << endl;
            }

        }
        if (DEBUG == true)
            cout << "Reverse" << endl;
        for (k = Data<Type>::sizes() - 1; k >= 0; k--) { // обратный ход
            temp = Data<Type>::vecB.at(k);
            for (j = Data<Type>::sizes() - 1; j > k; j--) {
                temp -= Data<Type>::matA.at(k).at(j) * Data<Type>::vecX.at(j);
            }
            Data<Type>::vecX.at(k) = temp / Data<Type>::matA.at(k).at(k);
            if (DEBUG == true)
                cout << "X[" << k << "] = " << Data<Type>::vecX.at(k) << endl;
        }
    }

	void SetLine3D(BitmapBuffer* buffer, Camera& cam, int i, int i1, BitmapRGB color) {
		std::vector <Type> P1 = { Data<Type>::readA(i, 0),  Data<Type>::readA(i, 1),  Data<Type>::readA(i, 2) };
		std::vector <Type> P2 = { Data<Type>::readA(i1, 0),  Data<Type>::readA(i1, 1),  Data<Type>::readA(i1, 2) };
		std::vector <Type> Cam = { cam.coordCam.x, cam.coordCam.y, cam.coordCam.z };
		std::vector <Type> intersect;
		int ind[12][3] = { 
			{3,0,1},
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
		int indEdge[12][2] = { {0, 1},
			{1, 2},
			{2, 3},
			{3, 0},
			{4, 5},
			{5, 6},
			{6, 7},
			{7, 4},
			{0, 4},
			{1, 5},
			{2, 6},
			{3, 7} };
		bool p1_ch[12] = { 0, 0, 0 ,0, 0, 0 , 0, 0, 0 , 0, 0, 0 }, p2_ch[12] = { 0,0,0,0, 0, 0 ,0, 0, 0 ,0, 0, 0  };
		bool check1 = 0, check2 = 0;
		for (int k = 0; k < 12; k++) {
			bool p1_check[3] = { 0, 0, 0 }, p2_check[3] = { 0, 0, 0 };
			bool p1_res = 1, p2_res = 1;
			for (int j = 0; j < 3; j++) {
				if (P1[0] != Data<Type>::readA(ind[k][j], 0) && P1[1] != Data<Type>::readA(ind[k][j], 1) && P1[2] != Data<Type>::readA(ind[k][j], 2)) p1_check[j] = 1;
				if (P2[0] != Data<Type>::readA(ind[k][j], 0) && P2[1] != Data<Type>::readA(ind[k][j], 1) && P2[2] != Data<Type>::readA(ind[k][j], 2)) p2_check[j] = 1;
				p1_res += p1_check[j];
				p2_res += p2_check[j];
			}
			if (p1_res && p2_res) {
				vector<vector<Type>> tri = { {Data<Type>::readA(ind[k][0], 0), Data<Type>::readA(ind[k][0], 1), Data<Type>::readA(ind[k][0], 2)},
					{Data<Type>::readA(ind[k][1], 0), Data<Type>::readA(ind[k][1], 1), Data<Type>::readA(ind[k][1], 2)}, 
					{Data<Type>::readA(ind[k][2], 0), Data<Type>::readA(ind[k][2], 1), Data<Type>::readA(ind[k][2], 2)} };
				if (lineSegIntersectTri(tri, P1, Cam, intersect))  p1_ch[k] = 1;
				if (lineSegIntersectTri(tri, P2, Cam, intersect)) p2_ch[k] = 1;
			}
			check1 += p1_ch[k];
			check2 += p2_ch[k];
		}
		/*
		for (int j = 0; j < 12; j++) {
			if (pointInPolygon(i, (int*)indEdge[j], cam) == TOUCHING) {
				cnt1[j]++;
			}
			if (pointInPolygon(i1, (int*)indEdge[j], cam) == TOUCHING) {
				cnt2[j]++;
			}
		}*/
		if (!check1 && !check2) {
			Data<Type>::savX(i, 1);
			vector<Type> D(P2.size());
			for (size_t i = 0; i < P2.size(); ++i)
				D[i] = P2[i] - P1[i];
			Type d, x1, x2, y1, y2, z1, z2, dx, dy, dz, ax, ay, az, sx, sy, sz, x, y, z, ind, zd, yd, xd, idx;
			for (unsigned int i = 0; i < D.size(); i++)
			{
				if (D[i] < 0) {
					D[i] *= -1; //make positive.    _OR_   use D[i] = abs(D[i]);
					D[i]++;
				}
			}
			d = *std::max_element(D.begin(), D.end());
			std::vector <Type>X;
			std::vector <Type>Y;
			std::vector <Type>Z;

			x1 = P1[0];
			y1 = P1[1];
			z1 = P1[2];

			x2 = P2[0];
			y2 = P2[1];
			z2 = P2[2];

			dx = x2 - x1;
			dy = y2 - y1;
			dz = z2 - z1;

			ax = std::abs(dx) * 2;
			ay = std::abs(dy) * 2;
			az = std::abs(dz) * 2;


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

					if (std::round(x) == std::round(x2)) {
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

						if (std::round(y) == std::round(y2))
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

							if (std::round(z) == std::round(z2))
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
			for (int k = 0; k < X.size(); k++) {
				SetPoint(buffer, X[k], Y[k], color);
			}
			X.clear();
			Y.clear();
			Z.clear();
			X.shrink_to_fit();
			Y.shrink_to_fit();
			Z.shrink_to_fit();

		}
		else {
			Data<Type>::savX(i, 0);
		}
	}

	void Carryover(char ind, double angle) {
		Type Dot[2];
		Type Dx1, Dy1;
		int a[4] = { 0,
			1,
			6,
			7 };
		if (cross(Dot, a)) {
			Dx1 = Dot[0];
			Dy1 = Dot[1];
			for (int i = 0; i < Data<Type>::sizes(); i++) {
				const int N = 4; //колвo строк
				const int M = 1; //колво столбцов результата
				Type preCoord[N][M] = { {Data<Type>::readA(i, 0)},
											{Data<Type>::readA(i, 1)},
											{Data<Type>::readA(i, 2)},
											{1} };
				Type CarryMatr[N][N] = { {1, 0 , 0, 0},
											{0, 1 , 0, 0},
											{0, 0 , 1, 0},
											{-Dx1, -Dy1, 0, 1} };
				Type Prod[N][M] = { {0},
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
				Data<Type>::savA(i, 0, Prod[0][0]);
				Data<Type>::savA(i, 1, Prod[1][0]);
				Data<Type>::savA(i, 2, Prod[2][0]);
			}
			switch (ind)
			{
			case 'y':
				Ry(angle);
				break;
			case 'x':
				Rx(angle);
				break;
			case 'z':
				Rz(angle);
				break;
			}

			for (int i = 0; i < Data<Type>::sizes(); i++) {
				const int N = 4; //колвo строк
				const int M = 1; //колво столбцов результата
				Type preCoord[N][M] = { {Data<Type>::readA(i, 0)},
												{Data<Type>::readA(i, 1)},
												{Data<Type>::readA(i, 2)},
												{1} };
				Type CarryMatr[N][N] = { {1, 0 , 0, 0},
											{0, 1 , 0, 0},
											{0, 0 , 1, 0},
											{Dx1, Dy1, 0, 1} };
				Type Prod[N][M] = { {0},
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
				Data<Type>::savA(i, 0, Prod[0][0]);
				Data<Type>::savA(i, 1, Prod[1][0]);
				Data<Type>::savA(i, 2, Prod[2][0]);
			}
		}
	}

	void Rx(double angle) //поворот вокруг оси X
	{
		const int N = 4; //колвo строк
		const int M = 1; //колво столбцов результата
		for (int i = 0; i < Data<Type>::sizes(); i++) {
			Type preCoord[N][M] = { {Data<Type>::readA(i, 0)},
										{Data<Type>::readA(i, 1)},
										{Data<Type>::readA(i, 2)},
										{1} };
			Type CarryMatr[N][N] = { {1, 0, 0, 0},
										{0, std::cos(angle * M_PI / 180.0) , -std::sin(angle * M_PI / 180.0), 0},
										{0, std::sin(angle * M_PI / 180.0) , std::cos(angle * M_PI / 180.0), 0},
										{0, 0, 0, 1} };
			Type Prod[N][M] = { {0},
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
			Data<Type>::savA(i, 0, Prod[0][0]);
			Data<Type>::savA(i, 1, Prod[1][0]);
			Data<Type>::savA(i, 2, Prod[2][0]);
		}
	}

	void Ry(double angle) //поворот вокруг оси Y
	{
		const int N = 4; //колвo строк
		const int M = 1; //колво столбцов результата
		for (int i = 0; i < Data<Type>::sizes(); i++) {
			Type preCoord[N][M] = { {Data<Type>::readA(i, 0)},
										{Data<Type>::readA(i, 1)},
										{Data<Type>::readA(i, 2)},
											{1} };
			Type CarryMatr[N][N] = { {std::cos(angle * M_PI / 180.0), 0, std::sin(angle * M_PI / 180.0), 0},
										{0, 1 , 0, 0},
										{-std::sin(angle * M_PI / 180.0), 0 , std::cos(angle * M_PI / 180.0), 0},
										{0, 0, 0, 1} };
			Type Prod[N][M] = { {0},
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
			Data<Type>::savA(i, 0, Prod[0][0]);
			Data<Type>::savA(i, 1, Prod[1][0]);
			Data<Type>::savA(i, 2, Prod[2][0]);
		}
	}

	void Rz(double angle) //поворот вокруг оси Z
	{
		const int N = 4; //колвo строк
		const int M = 1; //колво столбцов результата
		for (int i = 0; i < Data<Type>::sizes(); i++) {
			Type preCoord[N][M] = { {Data<Type>::readA(i, 0)},
										{Data<Type>::readA(i, 1)},
										{Data<Type>::readA(i, 2)},
										{1} };
			Type CarryMatr[N][N] = { {std::cos(angle * M_PI / 180.0), std::sin(angle * M_PI / 180.0) , 0, 0},
										{-std::sin(angle * M_PI / 180.0), std::cos(angle * M_PI / 180.0) , 0, 0},
										{0, 0 , 1, 0},
										{0, 0, 0, 1} };
			Type Prod[N][M] = { {0},
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
			Data<Type>::savA(i, 0, Prod[0][0]);
			Data<Type>::savA(i, 1, Prod[1][0]);
			Data<Type>::savA(i, 2, Prod[2][0]);
		}
	}

	bool cross(Type* dot, int* ind) {
		Type n;
		if (Data<Type>::readA(ind[2], 1) - Data<Type>::readA(ind[0], 1) != 0) {  // a(y)
			Type q = (Data<Type>::readA(ind[2], 0) - Data<Type>::readA(ind[0], 0)) / (Data<Type>::readA(ind[0], 1) - Data<Type>::readA(ind[2], 1));
			Type sn = (Data<Type>::readA(ind[1], 0) - Data<Type>::readA(ind[3], 0)) + (Data<Type>::readA(ind[1], 1) - Data<Type>::readA(ind[3], 1)) * q; 
			if (!sn) { return 0; }  // c(x) + c(y)*q
			Type fn = (Data<Type>::readA(ind[1], 0) - Data<Type>::readA(ind[0], 0)) + (Data<Type>::readA(ind[1], 1) - Data<Type>::readA(ind[0], 1)) * q;   // b(x) + b(y)*q
			n = fn / sn;
		}
		else {
			if (!(Data<Type>::readA(ind[1], 1) - Data<Type>::readA(ind[3], 1))) { return 0; }  // b(y)
			n = (Data<Type>::readA(ind[1], 1) - Data<Type>::readA(ind[0], 1)) / (Data<Type>::readA(ind[1], 1) - Data<Type>::readA(ind[3], 1));   // c(y)/b(y)
		}
		dot[0] = Data<Type>::readA(ind[1], 0) + (Data<Type>::readA(ind[3], 0) - Data<Type>::readA(ind[1], 0)) * n;  // x3 + (-b(x))*n
		dot[1] = Data<Type>::readA(ind[1], 1) + (Data<Type>::readA(ind[3], 1) - Data<Type>::readA(ind[1], 1)) * n;  // y3 +(-b(y))*n
		return 1;
	}

	int pointInPolygon(int a, int* indEdge, Camera& cam)
	{
		int parity = 0;
		Type e[2][2] = { { Data<Type>::readA(indEdge[0], 0), Data<Type>::readA(indEdge[0], 1)},
						{ Data<Type>::readA(indEdge[1], 0), Data<Type>::readA(indEdge[1], 1)} };
		switch (edgeType(a, e, cam)) {
		case TOUCHING:
			return BOUNDARY;
		case CROSSING:
			parity = 1 - parity;
		}
		return (parity ? INSIDE : OUTSIDE);
	}
	
	int edgeType(int a, Type e[2][2], Camera& cam)
	{
		Type v[2] = { e[0][0], e[0][1] };
		Type w[2] = { e[1][0], e[1][1] };
		switch (classify(a, e, cam)) {
		case LEFT:
			return ((v[1] < Data<Type>::readA(a, 1)) && (Data<Type>::readA(a, 1) <= w[1])) ? CROSSING : INESSENTIAL;
		case RIGHT:
			return ((w[1] < Data<Type>::readA(a, 1)) && (Data<Type>::readA(a, 1) <= v[1])) ? CROSSING : INESSENTIAL;
		case BETWEEN:
		case ORIGIN:
		case DESTINATION:
			return TOUCHING;
		default:
			return INESSENTIAL;
		}
	}

	int classify(int i,Type e[2][2], Camera& cam)
	{
		Type a[2] = { e[1][0] - e[0][0], e[1][1] - e[0][1] };
		Type b[2] = { cam.coordCam.x - e[0][0], cam.coordCam.y - e[0][1] };
		double sa = a[0] * b[1] - b[0] * a[1];
		if (sa > 0.0)
			return LEFT;
		if (sa < 0.0)
			return RIGHT;
		if ((a[0] * b[0] < 0.0) || (a[1] * b[1] < 0.0))
			return BEHIND;
		if (std::sqrt(a[0]* a[0] + a[1] * a[1]) < std::sqrt(b[0] * b[0] + b[1] * b[1]))
			return BEYOND;
		if ((e[0][0] == a[0]) && (a[1] == e[0][1]))
			return ORIGIN;
		if ((e[1][0] == a[0]) && (e[1][1] == a[1]))
			return DESTINATION;
		return BETWEEN;
	}

	bool LineFacet(int ind[3], vector<Type>& A, vector<Type>& B) {
		double d;
		double a1, a2, a3;
		double total, denom, mu;
		Type p2[3] = { A[0],A[1], A[2] }; //вершина 
		Type p1[3] = { B[0],B[1], B[2] }; //точка камеры 
		
		std::vector<Type> n;
		std::vector<Type> p;
		std::vector<Type> pa1; 
		std::vector<Type> pa2;
		std::vector<Type> pa3;

		//pointer[ind[0]] - 0
		//pointer[ind[1]] - 1
		//pointer[ind[2]] - 2
		/* Рассчитайте размеры плоскости */

		n.push_back((Data<Type>::readA(ind[0], 1) - Data<Type>::readA(ind[1], 1)) * (Data<Type>::readA(ind[2], 2) - Data<Type>::readA(ind[1], 2)) - (Data<Type>::readA(ind[0], 2) - Data<Type>::readA(ind[1], 2)) * (Data<Type>::readA(ind[2], 1) - Data<Type>::readA(ind[1], 1)));
		n.push_back((Data<Type>::readA(ind[0], 2) - Data<Type>::readA(ind[1], 2)) * (Data<Type>::readA(ind[2], 0) - Data<Type>::readA(ind[1], 0)) - (Data<Type>::readA(ind[0], 0) - Data<Type>::readA(ind[1], 0)) * (Data<Type>::readA(ind[2], 2) - Data<Type>::readA(ind[1], 2)));
		n.push_back((Data<Type>::readA(ind[0], 0) - Data<Type>::readA(ind[1], 0)) * (Data<Type>::readA(ind[2], 1) - Data<Type>::readA(ind[1], 1)) - (Data<Type>::readA(ind[0], 1) - Data<Type>::readA(ind[1], 1)) * (Data<Type>::readA(ind[2], 0) - Data<Type>::readA(ind[1], 0)));
		
		Normalize(n);
		
		d = -n[0] * Data<Type>::readA(ind[1], 0) - n[1] * Data<Type>::readA(ind[1], 1) - n[2] * Data<Type>::readA(ind[1], 2);

		/* Вычислите положение на прямой, пересекающей плоскость */
		denom = n[0] * (p2[0] - p1[0]) + n[1] * (p2[1] - p1[1]) + n[2] * (p2[2] - p1[2]);
		if (std::abs(denom) < EPS)         /* Линия и плоскость не пересекаются 390 340*/
			return(FALSE);
		mu = -(d + n[0] * p1[0] + n[1] * p1[1] + n[2] * p1[2]) / denom;
		p.push_back(p1[0] + mu * (p2[0] - p1[0]));
		p.push_back(p1[1] + mu * (p2[1] - p1[1]));
		p.push_back(p1[2] + mu * (p2[2] - p1[2]));
		if (mu < 0 || mu > 1)   /* Пересечение не вдоль отрезка прямой */
			return(FALSE);

		/* Определите, ограничена ли точка пересечения pointer[int[1]],pointer[ind[0]],pointer[int[2]] */
		pa1.push_back(Data<Type>::readA(ind[1], 0) - p[0]);
		pa1.push_back(Data<Type>::readA(ind[1], 1) - p[1]);
		pa1.push_back(Data<Type>::readA(ind[1], 2) - p[2]);
		
		Normalize(pa1);
		
		pa2.push_back(Data<Type>::readA(ind[0], 0) - p[0]);
		pa2.push_back(Data<Type>::readA(ind[0], 1) - p[1]);
		pa2.push_back(Data<Type>::readA(ind[0], 2) - p[2]);
		
		Normalize(pa2);
		
		pa3.push_back(Data<Type>::readA(ind[2], 0) - p[0]);
		pa3.push_back(Data<Type>::readA(ind[2], 1) - p[1]);
		pa3.push_back(Data<Type>::readA(ind[2], 2) - p[2]);
		
		Normalize(pa3);
		
		a1 = pa1[0] * pa2[0] + pa1[1] * pa2[1] + pa1[2] * pa2[2];
		a2 = pa2[0] * pa3[0] + pa2[1] * pa3[1] + pa2[2] * pa3[2];
		a3 = pa3[0] * pa1[0] + pa3[1] * pa1[1] + pa3[2] * pa1[2];
		total = (std::acos(a1) + std::acos(a2) + std::acos(a3)) * (180.0 / M_PI);
		if (std::abs(total - 360) > EPS) //10 46.7
			return(FALSE);

		return(TRUE);
	}

#define EPSILON 0.000001f
	bool lineSegIntersectTri(
		vector<vector<Type>> tri,
		vector<Type>& A,
		vector<Type>& B,
		vector<Type>& interst
	) {
		std::vector <Type> e0 = { tri[1][0] - tri[0][0], tri[1][1] - tri[0][1], tri[1][2] - tri[0][2] };
		std::vector <Type> e1 = { tri[2][0] - tri[0][0], tri[2][1] - tri[0][1], tri[2][2] - tri[0][2] };

		std::vector <Type> dir = { B[0] - A[0], B[1] - A[1], B[2] - A[2] };
		double c=1;
		if (abs(dir) != 0)
			c = abs(dir);
		std::vector <Type> dir_norm = { dir[0] / c, dir[1] / c, dir[2] / c };
		std::vector <Type> h = crosss(dir_norm, e1);
		const float a = e0[0] * h[0] + e0[1] * h[1] + e0[2] * h[2];

		if (a > -EPSILON && a < EPSILON) {
			return false;
		}

		std::vector <Type> s = { A[0] - tri[0][0], A[1] - tri[0][1], A[2] - tri[0][2] };
		const float f = 1.0f / a;
		const float u = f * (s[0] * h[0] + s[1] * h[1] + s[2] * h[2]);

		if (u < 0.0f || u > 1.0f) {
			return false;
		}

		std::vector <Type> q = crosss(s, e0);
		const float v = f * (dir_norm[0] * q[0] + dir_norm[1] * q[1] + dir_norm[2] * q[2]);

		if (v < 0.0f || u + v > 1.0f) {
			return false;
		}

		const float t = f * (e1[0] * q[0] + e1[1] * q[1] + e1[2] * q[2]);

		if (t > EPSILON && t < std::sqrtf(dir[0]*dir[0]+dir[1]*dir[1]+dir[2]*dir[2])) { // segment intersection
			if (interst.size()) {
				for (int i = 0; i < 3; i++) {
					dir_norm[i] *= t;
					interst[i] = A[i] + dir_norm[i];
				}
			}

			return true;
		}

		return false;
	}

	vector<Type>  crosss(const std::vector <Type>& a, const std::vector <Type>& b) {
		vector<Type> ab{ a[1] * b[2] - a[2] * b[1] ,
						  a[2] * b[0] - a[0] * b[2],
							a[0] * b[1] - a[1] * b[0] };
		return ab;
	}

	double Normalize(std::vector<Type>& a) {
		double c = 0;
		double sum = 0;
		for (int i = 0; i < a.size(); i++) {
			sum += a[i] * a[i];
		}
		c = std::sqrt(sum);
		if (c != 0 && c != 1) {
			for (int i = 0; i < a.size(); i++)
				a[i] /= c;
		}
		//c = std::sqrt(sum);
		return c;
	}

	double abs(std::vector<Type>& a) {
		double c = 0;
		double sum = 0;
		for (int i = 0; i < a.size(); i++) {
			sum += a[i] * a[i];
		}
		c = std::sqrt(sum);
		return c;
	}
	void EnterSketch(BitmapBuffer* buffer, unsigned int width, unsigned int height, int* ind, BitmapRGB OldColor, BitmapRGB NewColor) {
		bool check[4] = { 0, 0, 0, 0 };
			for (int i = 0; i < 4; i++) {
				if ((Data<Type>::readA(ind[i], 1) < height) &&
					(Data<Type>::readA(ind[i], 1) > 0) &&
					(Data<Type>::readA(ind[i], 0) < width) &&
					(Data<Type>::readA(ind[i], 0) > 0))
				{
					if (Data<Type>::readX(ind[i])) check[i] = 1;
				}
			}
			if (check[0] && check[1] && check[2] && check[3]) {
				Type Dot[2];
				int counter = 0;
				if (cross(Dot, ind))
					Sketch(buffer, width, height, round(Dot[0]), round(Dot[1]), OldColor, NewColor, counter);
			}
	}
	void Sketch(BitmapBuffer* buffer, unsigned int width, unsigned int height, int x, int y, BitmapRGB OldColor, BitmapRGB NewColor, int cnt)
	{
		if((y < height) &&
			(y > 0) &&
			(x < width) &&
			(x > 0) && cnt < 3000)
		{
			
			if (((*buffer)[y][x].red == OldColor.red) &&
				((*buffer)[y][x].green == OldColor.green) &&
				((*buffer)[y][x].blue == OldColor.blue))
			{
				cnt++;
				SetPointS(buffer, width, height, x, y, NewColor);
				Sketch(buffer, width, height, x + 1, y, OldColor, NewColor, cnt);
				Sketch(buffer, width, height, x - 1, y, OldColor, NewColor, cnt);
				Sketch(buffer, width, height, x, y - 1, OldColor, NewColor, cnt);
				Sketch(buffer, width, height, x, y + 1, OldColor, NewColor, cnt);
			}
		}
	}

	void SetPointS(BitmapBuffer* buffer, unsigned int width, unsigned int height, int x, int y, BitmapRGB color)
	{
		if (y < height &&
			y > 0 && x < width && x > 0)
		{
			(*buffer)[y][x].red = color.red;
			(*buffer)[y][x].green = color.green;
			(*buffer)[y][x].blue = color.blue;
		}
	}

	/*
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
	*/
	void EnterShadow(BitmapBuffer* buffer, Camera& lithg, Earth* earth, vector<vector<double>>& CoordShadow) {
		for (int k = 0; k < 8; k++) {
			bool check[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			int ind[12][3] = {
				{3,0,1},
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
			vector<Type> interst;
			vector<Type> B = { lithg.coordLight.x, lithg.coordLight.y, lithg.coordLight.z };
			vector<Type> A = { Data<Type>::readA(k, 0), Data<Type>::readA(k, 1), Data<Type>::readA(k, 2) };
			bool checks = 0;
			for (int i = 0; i < 12; i++) {
				bool p_check[3] = { 0, 0, 0 };
				bool p_res = 1;
				for (int j = 0; j < 3; j++) {
					if (A[0] != Data<Type>::readA(ind[i][j], 0) && A[1] != Data<Type>::readA(ind[i][j], 1) && A[2] != Data<Type>::readA(ind[i][j], 2)) p_check[j] = 1;
					p_res += p_check[j];
				}
				if (p_res) {
					vector<vector<Type>> tri;
					tri.push_back(vector<Type>());
					tri.push_back(vector<Type>()); 
					tri.push_back(vector<Type>()); 
					tri.at(0).push_back(Data<Type>::readA(ind[i][0], 0));
					tri.at(0).push_back(Data<Type>::readA(ind[i][0], 1));
					tri.at(0).push_back(Data<Type>::readA(ind[i][0], 2));
					tri.at(1).push_back(Data<Type>::readA(ind[i][1], 0));
					tri.at(1).push_back(Data<Type>::readA(ind[i][1], 1));
					tri.at(1).push_back(Data<Type>::readA(ind[i][1], 2));
					tri.at(2).push_back(Data<Type>::readA(ind[i][2], 0));
					tri.at(2).push_back(Data<Type>::readA(ind[i][2], 1));
					tri.at(2).push_back(Data<Type>::readA(ind[i][2], 2));
					if (lineSegIntersectTri(tri, A, B, interst)) check[i] = 1;
					tri.at(0).clear();
					tri.at(1).clear();
					tri.at(2).clear();
					tri.clear();
				}
				checks += check[i];
			}
			if (!checks) {
				vector<Type> interst1 = { 0, 0, 0 };
				vector<Type> interst2 = { 0, 0, 0 };
				vector<vector<Type>> tri1 = { {earth[0].coordEarth.x, earth[0].coordEarth.y, earth[0].coordEarth.z},
									{earth[1].coordEarth.x, earth[1].coordEarth.y, earth[1].coordEarth.z},
									{earth[2].coordEarth.x, earth[2].coordEarth.y, earth[2].coordEarth.z} };
				vector<vector<Type>> tri2 = { {earth[0].coordEarth.x, earth[0].coordEarth.y, earth[0].coordEarth.z},
											{earth[2].coordEarth.x, earth[2].coordEarth.y, earth[2].coordEarth.z},
											{earth[3].coordEarth.x, earth[3].coordEarth.y, earth[3].coordEarth.z} };
				if (lineSegIntersectTri(tri1, A, B, interst1)) {
					CoordShadow.push_back(vector<double>());
					CoordShadow.at(CoordShadow.size() - 1).push_back(interst1[0]);
					CoordShadow.at(CoordShadow.size() - 1).push_back(interst1[1]);
					CoordShadow.at(CoordShadow.size() - 1).push_back(interst1[2]);
				}
				else {
					if (lineSegIntersectTri(tri2, A, B, interst2)) {
						CoordShadow.push_back(vector<double>());
						CoordShadow.at(CoordShadow.size() - 1).push_back(interst2[0]);
						CoordShadow.at(CoordShadow.size() - 1).push_back(interst2[1]);
						CoordShadow.at(CoordShadow.size() - 1).push_back(interst2[2]);
					}
				}
			}
		}
		
	}
};
