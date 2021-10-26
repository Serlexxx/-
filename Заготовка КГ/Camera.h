#pragma once
#include <iostream>
#include <math.h>
class Camera {
public:
	struct coordCam
	{
		double x = 0; double  y = 0; double z = 0;
	} coordCam;
	double GetfFOV();
	void SetCoordCam(double height, double width);
private:
	double fFOV = 3.1415 / 2;	//угол обзора
	double fDepth = 30.f;		//дистанция обзора
	
};