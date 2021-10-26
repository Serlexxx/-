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
 * \brief Установка пикселя
 * \param buffer Буфер кадра (указатель на массив)
 * \param x Положение по оси X
 * \param y Положение по ост Y
 * \param w Ширина фрейм-буфера
 * \param color Очистка цвета
 */
void SetPoint(BitmapBuffer* buffer, int x, int y, BitmapRGB color = { 0,0,0 });


/**
 * \brief Отрисовка кадра
 * \param width Ширина
 * \param height Высота
 * \param pixels Массив пикселов
 * \param hWnd Хендл окна, device context которого будет использован
 */
void PresentFrame(uint32_t width, uint32_t height, void* pixels, HWND hWnd);

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
void SetLine(BitmapBuffer* buffer, Point* point, Apex& apex, Camera& camera, BitmapRGB color = { 0,0,0 });



void CentralProjection(Camera& camera, Apex& apex, Point* pointer); //получает координату камеры по оси Z
double Determinant(double** T, int N); //детерминант
void Get_matrix(double** matrix, int n, double** temp_matrix, int indRow, int indCol);
void TransnMatrixx(double** matrix, double** tMatrix, int n);
double** dynamic_array_alloc(size_t N, size_t M);
void dynamic_array_free(double** A, size_t N);