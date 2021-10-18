#pragma once
#include <iostream>

class Camera {
public:
	struct coordCam
	{
		float x = 0; float  y = 0; float z = 0;
	} coordCam;

	void SetCoordCam(int height, int width);
private:
	float fFOV = 3.1415 / 3;	//угол обзора
	float fDepth = 30.f;		//дистанция обзора
	
};