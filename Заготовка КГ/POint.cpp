#include "Point.h"
using namespace std;
void Point::getApex(int Apex) {
	APEX = (Apex > 0 ? Apex : 3);
}

int Point::deApex(int count) {
	APEX -= count;
	return APEX;
}

void Point::getPoint() {
	for (int i = 0; i++; i < APEX-1) {
		cin >> pointer[i].x >> pointer[i].y;
	}
}

int Point::setApex() {
	return APEX;
}