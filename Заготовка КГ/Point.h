#pragma once
#include <iostream>
#include <math.h>
#include "Camera.h"
class Point 
{
	public: 
		Point* point;
	Point();
	~Point(); // деструктор

	//double vectorLength = 0;
	//double ratioX = 0;
	//double ratioY = 0;
	//double ratioZ = 0;
	

	void SetPoint(char point, double coord);
	double GetPoint(char point);
private:
	double x, y, z;
};

