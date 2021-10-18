#include "Point.h"
#define PLUS_X 20
#define PLUS_Y 200
#define Z 0.2 //Кооридината проэцирующей плоскости
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

void Point::CentralProjection(Camera& camera) {
	for (int i = 0; i < APEX; i++) {
		//GetVectorLength(pointer->x, pointer->y, pointer->z, camera);
		float prMatrix[4][4] = { {Z, 0, 0, 0},
								 {0, Z, 0, 0},
								 {0, 0, Z, 0},
								 {0, 0, 0, 1}
								};
		float xyzMatrix[4][1] = { {pointer[i].x}, pointer[i].y, pointer[i].x, {1} };
		float product[4][1] = { {0}, {0}, {0}, {0}};
		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 1; col++) {
				for (int inner = 0; inner < 4; inner++) {
					product[row][col] += prMatrix[row][inner] * xyzMatrix[inner][col];
				}
			}
		}
		pointer_pr[i].x = product[0][0];
		pointer_pr[i].y = product[1][0];
		pointer_pr[i].z = product[2][0];
	}
}

void Point::GetVectorLength(float x, float y, float z, Camera&camera) {
	ratio = sqrt(pow(camera.coordCam.x - x, 2) +
		pow(camera.coordCam.y - y, 2) +
		pow(camera.coordCam.z - z, 2)) /sqrt(pow(camera.coordCam.x - x, 2) +
						pow(camera.coordCam.y - y, 2) +
						pow(camera.coordCam.z - z, 2));
}

