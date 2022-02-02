#pragma once
#include <iostream>
#include <math.h>
#include "Camera.h"
struct Point{
	double x, y, z;
	
};

class PointBuffer 
{
public: 
	double GetPoint(char point_);
	PointBuffer(int apex);
	~PointBuffer(); // деструктор
	Point& operator[](int y);
	//PointBuffer& operator=(const PointBuffer& point_);
	
private:
	Point* point;
};

struct Facet {
	double x1, y1, z1;
	double x2, y2, z2;
	double x3, y3, z3;
	double x4, y4, z4;
};

class Facets {
public:
	Facets(int cnt);
	~Facets();
	Facet& operator[](int y);
private:
	Facet* facet;
};

