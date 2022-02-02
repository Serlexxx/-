#pragma once
#include <iostream>
#include <math.h>
class Camera {
public:
	struct coordCam
	{
		double x = 0; double  y = 0; double z = 0;
	} coordCam;
	struct coordLight
	{
		double x = 0; double  y = 0; double z = 0;
	} coordLight;
	
	void SetCoordCam(double width, double height);
private:
	double fovx; // угол обзора по х
	double fovy; // угол обзора по y
	double fDepth = 30.f;		//дистанция обзора
	
};