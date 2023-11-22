#include "Point.h"
#define PLUS_X 20
#define PLUS_Y 200
#define Z 0 //Кооридината проэцирующей плоскости

PointBuffer::PointBuffer(int apex) {
	point = (Point*)malloc(apex * sizeof(Point));
}


PointBuffer::~PointBuffer() {
	free(point);
}





double PointBuffer::GetPoint(char point_) {
	switch (point_)
	{
	case 'x':
		return point->x;
		break;
	case 'y':
		return point->y;
		break;
	case 'z':
		return point->z;
		break;
	}
}


Point& PointBuffer::operator[](int y)
{
	return this->point[y];
}

Facets::Facets(int apex) {
	facet = (Facet*)malloc(apex * sizeof(Facet));
}


Facets::~Facets() {
	free(facet);
}

Facet& Facets::operator[](int y)
{
	return this->facet[y];
}