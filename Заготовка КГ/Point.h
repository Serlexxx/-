#pragma once
#include <iostream>

class Point 
{
	public: 	
	// —труктура точки и массива точек фигуры
	struct pointBrenz {
		double x = 0, y = 0;
	} lineBrenz;
	pointBrenz* pointer = new pointBrenz[APEX];

	void getPoint();
	void getApex(int Apex); //конструктор с параметром
	int setApex();
	int deApex(int count);
private:
	//  ол-во вершин фигуры
	int APEX;
};