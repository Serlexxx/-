#pragma once
#include <iostream>

class Point 
{
	public: 	
	// ��������� ����� � ������� ����� ������
	struct pointBrenz {
		double x = 0, y = 0;
	} lineBrenz;
	pointBrenz* pointer = new pointBrenz[APEX];

	void getPoint();
	void getApex(int Apex); //����������� � ����������
	int setApex();
	int deApex(int count);
private:
	// ���-�� ������ ������
	int APEX;
};