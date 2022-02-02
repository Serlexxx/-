#pragma once

//#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <math.h>
#include <vector>
#include <algorithm>

#include "BitmapBuffer.h"
#include "Point.h"
#include "Apex.h"
#include "Raycasting.h"
#include "Earth.h"

//enum { LEFT, RIGHT, BEYOND, BEHIND, BETWEEN, ORIGIN, DESTINATION };
//    СЛЕВА, СПРАВА, ВПЕРЕДИ, ПОЗАДИ, МЕЖДУ,   НАЧАЛО, КОНЕЦ

//int classify(const std::vector<double>& p0, const std::vector<double>& p1, const std::vector<double>& p2);
//bool pointInTriangle(const std::vector<double>& p, const std::vector<double>& a, const std::vector<double>& b, const std::vector<double>& c);
//void visovTeni(BitmapBuffer* buffer, PointBuffer& pointer, Camera& cam, Earth* earth, Ten* many);
void Rz(Apex& apex, Ten* pointer, double angle);
void Rx(Apex& apex, Ten* pointer, double angle);
/**
 * \brief Установка пикселя
 * \param buffer Буфер кадра (указатель на массив)
 * \param x Положение по оси X
 * \param y Положение по ост Y
 * \param w Ширина фрейм-буфера
 * \param color Очистка цвета
 */
void SetPoint(BitmapBuffer* buffer, int x, int y, BitmapRGB color = { 0,0,0 });

//void SetPoint3D(BitmapBuffer3D* buffer, int x, int y, int z, BitmapDataRGB color);
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
void SetLine(BitmapBuffer* buffer, std::vector<std::vector<double>> coordsharow, BitmapRGB color = { 0,0,0 });

void SetLine(BitmapBuffer* buffer, Earth* pointer, int apex, BitmapRGB color = { 0,0,0 });

//void SetLine3D(BitmapBuffer* buffer, PointBuffer& pointer, Camera& cam, int i, int i1, BitmapRGB color = { 0,0,0 });

void PutLine3D(BitmapBuffer* buffer, PointBuffer& pointer, int i, int i1, BitmapRGB color = { 0,0,0 });

//void Sketch(BitmapBuffer* buffer, double x, double y,  PointBuffer& point, BitmapRGB OldColor, BitmapRGB NewColor);
void SetLine3D(BitmapBuffer* buffer, std::vector<std::vector<double>> coordsharow, int i, int i1, BitmapRGB color);



void CentralProjection(Camera& camera, Apex& apex, PointBuffer& pointer); //получает координату камеры по оси Z

double abs(const std::vector<double>& a); //модуль вектора(длина)

bool proverka(BitmapBuffer* buffer, int x, int y, BitmapRGB color);
void visovSketch(BitmapBuffer* buffer, int* xy, PointBuffer& point, BitmapRGB OldColor, BitmapRGB NewColor);
//std::vector <double> operator-(const std::vector<double>& a, const std::vector<double>& b); //векторное вычитание

std::vector <double> operator^(const std::vector<double>& a, const std::vector<double>& b); //векторное произведение
double operator*(std::vector<double>& a, std::vector<double>& b); //скалярное произведение
std::vector <double> operator*(const std::vector<double>& a, double k); //умножение вектора на число
std::vector <double> operator/(const std::vector<double>& a, double k); //деление вектора на число
std::vector <double> operator+(const std::vector<double>& a, const std::vector<double>& b); //сложение векторов


void Carryover(Apex& apex, PointBuffer& pointer, char ind, double angle); // перенос в начало координат и поворот вокруг какой-то оси

bool cross(PointBuffer& pointer, double* dot, int* ind); //центр прямоугольника

bool pointInPolygon(PointBuffer& point, int x, int y);

double Determinant(double** T, int N); //детерминант
void Get_matrix(double** matrix, int n, double** temp_matrix, int indRow, int indCol);
void TransnMatrixx(double** matrix, double** tMatrix, int n);
double** dynamic_array_alloc(size_t N, size_t M);
void dynamic_array_free(double** A, size_t N);