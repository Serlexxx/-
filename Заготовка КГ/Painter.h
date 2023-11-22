#pragma once

//#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <math.h>
#include <vector>
#include <algorithm>

#include "BitmapBuffer.h"
#include "Earth.h"


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
void SetLine(BitmapBuffer* buffer, std::vector<std::vector<double>> coordsharow, BitmapRGB color = { 0,0,0 });

void SetLine3D(BitmapBuffer* buffer, std::vector<std::vector<double>> coordsharow, int i, int i1, BitmapRGB color);

double abs(const std::vector<double>& a); //������ �������(�����)
