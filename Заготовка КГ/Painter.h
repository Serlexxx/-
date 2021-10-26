#pragma once

//#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <math.h>
#include <vector>

#include "BitmapBuffer.h"
#include "Point.h"
#include "Apex.h"




/**
 * \brief ��������� �������
 * \param buffer ����� ����� (��������� �� ������)
 * \param x ��������� �� ��� X
 * \param y ��������� �� ��� Y
 * \param w ������ �����-������
 * \param color ������� �����
 */
void SetPoint(BitmapBuffer* buffer, int x, int y, BitmapRGB color = { 0,0,0 });


/**
 * \brief ��������� �����
 * \param width ������
 * \param height ������
 * \param pixels ������ ��������
 * \param hWnd ����� ����, device context �������� ����� �����������
 */
void PresentFrame(uint32_t width, uint32_t height, void* pixels, HWND hWnd);

/**
 * \brief ��������� ����� (������� �������, �������� ����������)
 * \param buffer ����� ����� (��������� �� ������)
 * \param x0 ��������� ����� (���������� X)
 * \param y0 ��������� ����� (���������� Y)
 * \param x1 �������� ����� (���������� X)
 * \param y1 �������� ����� (���������� Y)
 * \param w ������ �����-������
 * \param color ������� �����
 */
void SetLine(BitmapBuffer* buffer, Point* point, Apex& apex, Camera& camera, BitmapRGB color = { 0,0,0 });



void CentralProjection(Camera& camera, Apex& apex, Point* pointer); //�������� ���������� ������ �� ��� Z
double Determinant(double** T, int N); //�����������
void Get_matrix(double** matrix, int n, double** temp_matrix, int indRow, int indCol);
void TransnMatrixx(double** matrix, double** tMatrix, int n);
double** dynamic_array_alloc(size_t N, size_t M);
void dynamic_array_free(double** A, size_t N);