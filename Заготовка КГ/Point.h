#pragma once
#include <iostream>
#include <math.h>
#include "Camera.h"
class Point 
{
	public: 	
	// ��������� ����� � ������� ����� ������
	struct pointBrenz {
		double x = 0, y = 0, z = 10;
	};
	pointBrenz* pointer = new pointBrenz[APEX];

	//��������� ������������ ����� ������
	struct pointBrenz_pr {
		double x = 0, y = 0, z = 0;
	};
	pointBrenz_pr* pointer_pr = new pointBrenz_pr[APEX];
	float vectorLength = 0;
	float ratio = 0;
	void GetVectorLength(float x, float y, float z, Camera&camera);
	void CentralProjection(Camera& camera);; //�������� ���������� ������ �� ��� Z
	void GetPoint();
	void GetApex();
	int SetApex() const;
private:
	// ���-�� ������ ������
	int APEX;
};

