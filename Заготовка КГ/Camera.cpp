#include "Camera.h"


void Camera::SetCoordCam(int height, int width) {
	coordCam.x = width / 2;
	coordCam.y = height / 2;
	coordCam.z = 200;
}