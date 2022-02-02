#include "Raycasting.h"
/*
* int* ind - номера точек плоскости
* double* Y, double* X - точки луча(Y- проверяемая точка, X - источник света/камера)
*/
/*
bool Intersection(PointBuffer& pointer, int* ind, double* x, double* y) {
	std::vector<double> B = { pointer[ind[0]].x, pointer[ind[0]].y, pointer[ind[0]].z };
	std::vector<double> A = { pointer[ind[1]].x, pointer[ind[1]].y, pointer[ind[1]].z };
	std::vector<double> C = { pointer[ind[2]].x, pointer[ind[2]].y, pointer[ind[2]].z };
	std::vector<double> X = { x[0], x[1], x[2] };
	std::vector<double> Y = { y[0], y[1], y[2] };
	std::vector<double> O;
	std::vector<double> N = (B-A)^(C-A);
	//to do normalize
	if(abs(N)!=0)
		N = N / abs(N);
	std::vector<double> V = A - X;
	double d = N * V;

	std::vector<double> W = Y - X;
	double e = N * W;
	if (round(e) != 0) {
		O = X + W * d / e;
		std::vector<double> P1 = X - O;
		std::vector<double> P2 = Y - O;
		if (round((P1 * P2)) <= 0) {
			return 0;
		}
		else {
			if (round(d) == 0) {
				return 0;
			}
			else
			{
				return 1;
			}
		}
	}

}
*/
/*
bool IntersectionWithEart(Earth* pointer, int* ind, std::vector<double> X, std::vector<double> Y, std::vector<double>& O) {
	std::vector<double> B = { pointer[ind[0]].coordEarth.x, pointer[ind[0]].coordEarth.y, pointer[ind[0]].coordEarth.z };
	std::vector<double> A = { pointer[ind[1]].coordEarth.x, pointer[ind[1]].coordEarth.y, pointer[ind[1]].coordEarth.z };
	std::vector<double> C = { pointer[ind[2]].coordEarth.x, pointer[ind[2]].coordEarth.y, pointer[ind[2]].coordEarth.z };
	std::vector<double> N = (B - A) ^ (C - A);
	//to do normalize
	if (abs(N) != 0)
		N = N / abs(N);
	std::vector<double> V = A - X;
	double d = N * V;

	std::vector<double> W = Y - X;
	double e = N * W;
	if (round(e) != 0) {
		O = X + W * d / e;
		std::vector<double> P1 = X - O;
		std::vector<double> P2 = Y - O;
		if (round((P1 * P2)) <= 0) {
			return 0;
		}
		else {
			if (round(d) == 0) {
				return 0;
			}
			else
			{
				return 1;
			}
		}
	}

}
*/
/*
bool LineFacet(PointBuffer& pointer, int* ind, double* x, double* y) {
	double d;
	double a1, a2, a3;
	double total, denom, mu;
	double p2[3] = { x[0],x[1], x[2] };
	double p1[3] = { y[0],y[1], y[2] };
	std::vector<double> n;
	std::vector<double> p;
	std::vector<double> pa1;
	std::vector<double> pa2;
	std::vector<double> pa3;

	//pointer[ind[0]] - 0
	//pointer[ind[1]] - 1
	//pointer[ind[2]] - 2
	/* Рассчитайте размеры плоскости 
	
	n.push_back((pointer[ind[0]].y - pointer[ind[1]].y) * (pointer[ind[2]].z - pointer[ind[1]].z) - (pointer[ind[0]].z - pointer[ind[1]].z) * (pointer[ind[2]].y - pointer[ind[1]].y));
	n.push_back((pointer[ind[0]].z - pointer[ind[1]].z) * (pointer[ind[2]].x - pointer[ind[1]].x) - (pointer[ind[0]].x - pointer[ind[1]].x) * (pointer[ind[2]].z - pointer[ind[1]].z));
	n.push_back((pointer[ind[0]].x - pointer[ind[1]].x) * (pointer[ind[2]].y - pointer[ind[1]].y) - (pointer[ind[0]].y - pointer[ind[1]].y) * (pointer[ind[2]].x - pointer[ind[1]].x));
	if(abs(n)!=0)
		n = n / abs(n);
	d = -n[0] * pointer[ind[1]].x - n[1] * pointer[ind[1]].y - n[2] * pointer[ind[1]].z;

	/* Вычислите положение на прямой, пересекающей плоскость 
	denom = n[0] * (p2[0] - p1[0]) + n[1] * (p2[1] - p1[1]) + n[2] * (p2[2] - p1[2]);
	if (round(denom) <0)         /* Линия и плоскость не пересекаются 
		return(FALSE);
	mu = -(d + n[0] * p1[0] + n[1] * p1[1] + n[2] * p1[2]) / denom;
	p.push_back(p1[0] + mu * (p2[0] - p1[0]));
	p.push_back(p1[1] + mu * (p2[1] - p1[1]));
	p.push_back(p1[2] + mu * (p2[2] - p1[2]));
	if (mu < 0 || mu > 1)   /* Пересечение не вдоль отрезка прямой 
		return(FALSE);

	/* Определите, ограничена ли точка пересечения pointer[int[1]],pointer[ind[0]],pointer[int[2]] 
	pa1.push_back(pointer[ind[1]].x - p[0]);
	pa1.push_back(pointer[ind[1]].y - p[1]);
	pa1.push_back(pointer[ind[1]].z - p[2]);
	if(abs(pa1)!=0)
		pa1 = pa1 / abs(pa1);

	pa2.push_back(pointer[ind[0]].x - p[0]);
	pa2.push_back(pointer[ind[0]].y - p[1]);
	pa2.push_back(pointer[ind[0]].z - p[2]);
	if(abs(pa2)!=0)
		pa2 = pa2 / abs(pa2);
	
	pa3.push_back(pointer[ind[2]].x - p[0]);
	pa3.push_back(pointer[ind[2]].y - p[1]);
	pa3.push_back(pointer[ind[2]].z - p[2]);
	if(abs(pa3)!=0)
		pa3 = pa3 / abs(pa3);
	
	a1 = pa1[0] * pa2[0] + pa1[1] * pa2[1] + pa1[2] * pa2[2];
	a2 = pa2[0] * pa3[0] + pa2[1] * pa3[1] + pa2[2] * pa3[2];
	a3 = pa3[0] * pa1[0] + pa3[1] * pa1[1] + pa3[2] * pa1[2];
	total = (acos(a1) + acos(a2) + acos(a3)) * 180/ 3.1415;
	if (abs(total - 360) > 100)
		return(FALSE);

	return(TRUE);
}
*/
