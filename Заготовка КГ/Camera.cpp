#include "Camera.h"


void Camera::SetCoordCam(double width, double height) {
	coordCam.x = width / 2;
	coordCam.y = height / 2;
	coordCam.z = -500;
	fovx = 2*abs(coordCam.x/coordCam.z);
	fovy = 2 * abs(coordCam.y / coordCam.z);
}

