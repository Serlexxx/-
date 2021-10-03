#pragma once

//#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <math.h>
#include <vector>
#include "Point.h"

/**
 * \brief Создание буфера кадра (двумерный массив структур RGBQUAD)
 * \param width Ширина буфера кадра
 * \param height Высота буфера кадра
 * \param clearColor Изначальный цвет
 * \return Указатель на массив
 */
RGBQUAD* CreateFrameBuffer(uint32_t width, uint32_t height, RGBQUAD clearColor = { 0,0,0,0 });

/**
 * \brief Заполнение буфера изображения каким-то конкретным цветом
 * \param buffer Буфер кадра (указатель на массив)
 * \param pixelCount Кол-во пикселей в буфере
 * \param clearColor Цвет
 */
void ClearFrame(RGBQUAD* buffer, uint32_t pixelCount, RGBQUAD clearColor = { 0,0,0,0 });

/**
 * \brief Установка пикселя
 * \param buffer Буфер кадра (указатель на массив)
 * \param x Положение по оси X
 * \param y Положение по ост Y
 * \param w Ширина фрейм-буфера
 * \param color Очистка цвета
 */
void SetPoint(RGBQUAD* buffer, int x, int y, uint32_t w, RGBQUAD color = { 0,0,0,0 });

/**
 * \brief Рисование линии (быстрый вариант, алгоритм Брэзенхема)
 * \param buffer Буфер кадра (указатель на массив)
 * \param x0 Начальная точка (компонента X)
 * \param y0 Начальная точка (компонента Y)
 * \param x1 Конечная точка (компонента X)
 * \param y1 Конечная точка (компонента Y)
 * \param w Ширина фрейм-буфера
 * \param color Очистка цвета
 */
void SetLine(RGBQUAD* buffer, uint32_t w, Point& point, RGBQUAD color = { 0,0,0,0 });


 /**
  * \brief Отрисовка кадра
  * \param width Ширина
  * \param height Высота
  * \param pixels Массив пикселов
  * \param hWnd Хендл окна, device context которого будет использован
  */
void PresentFrame(HDC hdc, uint32_t width, uint32_t height, void* pixels, HWND hWnd);

/**
* \param x Начальная точка(компонента X)
* \param y Начальная точка(компонента Y)
* \param k  Коэффициент масштабирования линии
* \param height Высота рабочей области
* \param width Ширина рабочей области
* */
void MatrixMultiplication(double k, double height, double width, Point &point);

/**
* \param angle  Коэффициент поворота линии
* \param height Высота рабочей области
* \param width Ширина рабочей области
* */
void Rotate(double angle, double height, double width, Point&point);

//void V_FP1(RGBQUAD* buffer, uint32_t w, double* Pt, RGBQUAD color = { 0, 0, 0, 0 });

void swaping(double& one, double& two);
void swaping_int(int& one, int& two);

