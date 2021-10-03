#pragma once

//#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <math.h>
#include <vector>
#include "Point.h"

/**
 * \brief �������� ������ ����� (��������� ������ �������� RGBQUAD)
 * \param width ������ ������ �����
 * \param height ������ ������ �����
 * \param clearColor ����������� ����
 * \return ��������� �� ������
 */
RGBQUAD* CreateFrameBuffer(uint32_t width, uint32_t height, RGBQUAD clearColor = { 0,0,0,0 });

/**
 * \brief ���������� ������ ����������� �����-�� ���������� ������
 * \param buffer ����� ����� (��������� �� ������)
 * \param pixelCount ���-�� �������� � ������
 * \param clearColor ����
 */
void ClearFrame(RGBQUAD* buffer, uint32_t pixelCount, RGBQUAD clearColor = { 0,0,0,0 });

/**
 * \brief ��������� �������
 * \param buffer ����� ����� (��������� �� ������)
 * \param x ��������� �� ��� X
 * \param y ��������� �� ��� Y
 * \param w ������ �����-������
 * \param color ������� �����
 */
void SetPoint(RGBQUAD* buffer, int x, int y, uint32_t w, RGBQUAD color = { 0,0,0,0 });

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
void SetLine(RGBQUAD* buffer, uint32_t w, Point& point, RGBQUAD color = { 0,0,0,0 });


 /**
  * \brief ��������� �����
  * \param width ������
  * \param height ������
  * \param pixels ������ ��������
  * \param hWnd ����� ����, device context �������� ����� �����������
  */
void PresentFrame(HDC hdc, uint32_t width, uint32_t height, void* pixels, HWND hWnd);

/**
* \param x ��������� �����(���������� X)
* \param y ��������� �����(���������� Y)
* \param k  ����������� ��������������� �����
* \param height ������ ������� �������
* \param width ������ ������� �������
* */
void MatrixMultiplication(double k, double height, double width, Point &point);

/**
* \param angle  ����������� �������� �����
* \param height ������ ������� �������
* \param width ������ ������� �������
* */
void Rotate(double angle, double height, double width, Point&point);

//void V_FP1(RGBQUAD* buffer, uint32_t w, double* Pt, RGBQUAD color = { 0, 0, 0, 0 });

void swaping(double& one, double& two);
void swaping_int(int& one, int& two);

