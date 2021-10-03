#include "Point.h"
#define PLUS_X 20
#define PLUS_Y 200

void Point::GetApex() {
	APEX = 4;
}


void Point::GetPoint() {
	for (int i = 1; i < 3; i++) {
		switch (i)
		{
		case 1: {
			pointer[0].x = 10;
			pointer[1].x = 60;
			pointer[0].y = 10;
			pointer[1].y = 60;
			break;
		}
		case 2: {
			pointer[2].y = pointer[1].y + PLUS_Y;
			pointer[3].y = pointer[0].y + PLUS_Y;
			pointer[2].x = pointer[1].x + PLUS_X;
			pointer[3].x = pointer[0].x + PLUS_X;
			break;
		}
		}
	}
}

int Point::SetApex() const {
	return APEX;
}


