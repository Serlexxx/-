#pragma once
#include <iostream>

class Point 
{
	public: 	
	// ��������� ����� � ������� ����� ������
	struct pointBrenz {
		double x = 0, y = 0;
	};
	pointBrenz* pointer = new pointBrenz[APEX];

	void GetPoint();
	void GetApex();
	int SetApex() const;
private:
	// ���-�� ������ ������
	int APEX;
};

