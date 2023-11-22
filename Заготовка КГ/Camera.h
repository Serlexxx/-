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
	double fovx; // ���� ������ �� �
	double fovy; // ���� ������ �� y
	double fDepth = 30.f;		//��������� ������
	
};