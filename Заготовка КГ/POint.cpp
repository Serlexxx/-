#include "Point.h"
#define PLUS_X 20
#define PLUS_Y 200
#define Z 0 //Кооридината проэцирующей плоскости

Point::Point() {
	
}


Point::~Point() {

}



void Point::SetPoint(char point, double coord) {
	switch (point)
	{
	case 'x':
		x = coord;
		break;
	case 'y':
		y = coord;
		break;
	case 'z':
		z = coord;
		break;

	}
}

double Point::GetPoint(char point) {
	switch (point)
	{
	case 'x':
		return x;
		break;
	case 'y':
		return y;
		break;
	case 'z':
		return z;
		break;

	}
}
