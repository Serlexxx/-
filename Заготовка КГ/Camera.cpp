#include "Camera.h"


void Camera::SetCoordCam(double height, double width) {
	coordCam.x = width / 2;
	coordCam.y = height / 2;
	coordCam.z = width/(2*tan(GetfFOV()/2));
}

double Camera::GetfFOV() {
	return fFOV;
}