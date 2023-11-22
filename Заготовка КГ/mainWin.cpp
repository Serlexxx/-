// Заготовка КГ.cpp : Определяет точку входа для приложения.
//
using namespace std;
#include <iostream>
#include <sstream>
#include <windowsx.h>
#include <chrono>
#include <thread>
#include <ctime>   
#include "framework.h"
#include "Resource.h"

#include "BitmapBuffer.h"
#include "Painter.h"
#include "Camera.h"
#include "Earth.h"
#include "Data.h"
#include "aloritms.h"

#define MAX_LOADSTRING 100
#define ID_CREATES 160
#define ID_FIRSTCHILD	161
#define MOVEMENT 5
// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING] = L"Компьютерная графика";;                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING] = L"Компьютерная графика";            // имя класса главного окна

LPCWSTR szTitleEnter = {};                  // Текст строки заголовка
WCHAR szWindowClassEnter[MAX_LOADSTRING] = L"Ввод координат";

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

static POINTS pt; //движение мыши
static POINTS ptTemp;
static POINTS ptsPrevEnd;      // предыдущая конечная точка 
static BOOL fPrevLine = FALSE; // флажок предыдущего нажатия




//Добавление отрисовщика
Camera camera;
algorithm<double>* points1 = new algorithm<double>(8, 3);
algorithm<double>* points2 = new algorithm<double>(8, 3);
BitmapBuffer frameBuffer;
Earth earth[4];
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.





    // Инициализация глобальных строк
    LoadString(hInstance, IDS_APP_TITLE, NULL, MAX_LOADSTRING);
    LoadString(hInstance, IDC_MY, NULL, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));

    MSG msg;



    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {

            SendMessage(msg.hwnd, WM_PAINT, NULL, NULL);
            TranslateMessage(&msg);

            DispatchMessage(&msg);
        }
    }

    
   
        

    return (int)msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

   
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}



//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_LBUTTONDOWN:
        SetCapture(hWnd);
        pt = MAKEPOINTS(lParam);
        return 0;
    case WM_LBUTTONUP:
        fPrevLine = FALSE;
        ClipCursor(NULL);
        ReleaseCapture();
        return 0;
    case WM_MOUSEMOVE: {
        if (wParam & MK_LBUTTON) {

            ptTemp = MAKEPOINTS(lParam);
            double deltaX = (ptTemp.x - pt.x);
            double deltaY = (ptTemp.y - pt.y);
            double angle = 10;
            if (deltaY > 0) {
                points1->Carryover('x', deltaY);
                points2->Carryover('x', deltaY);
            }
            else {
                if (deltaY < 0) {
                    points1->Carryover('x', deltaY);
                    points2->Carryover('x', deltaY);
                }
            }

            if (deltaX > 0) {

                points1->Carryover('y', deltaX);
                points2->Carryover('y', deltaX);
            }
            else {
                if (deltaX < 0) {
                    points1->Carryover('y', deltaX);
                    points2->Carryover('y', deltaX);
                }
            }
            SendMessage(hWnd, WM_PAINT, NULL, NULL);
            fPrevLine = TRUE;
            ptsPrevEnd = ptTemp;
        }
        break;
    }
    case WM_CREATE: {
        points1->savA(0, 0, 50);
        points1->savA(0, 1, 100);
        points1->savA(0, 2, 100);

        points1->savA(1, 0, 150);
        points1->savA(1, 1, 100);
        points1->savA(1, 2, 100);

        points1->savA(2, 0, 150);
        points1->savA(2, 1, 150);
        points1->savA(2, 2, 100);

        points1->savA(3, 0, 50);
        points1->savA(3, 1, 150);
        points1->savA(3, 2, 100);
      
        points1->savA(4, 0, 150);
        points1->savA(4, 1, 110);
        points1->savA(4, 2, 300);

        points1->savA(5, 0, 250);
        points1->savA(5, 1, 110);
        points1->savA(5, 2, 300);

        points1->savA(6, 0, 250);
        points1->savA(6, 1, 160);
        points1->savA(6, 2, 300);

        points1->savA(7, 0, 150);
        points1->savA(7, 1, 160);
        points1->savA(7, 2, 300);
        
        points2->savA(0, 0, 350);
        points2->savA(0, 1, 100);
        points2->savA(0, 2, 100);

        points2->savA(1, 0, 450);
        points2->savA(1, 1, 100);
        points2->savA(1, 2, 100);

        points2->savA(2, 0, 450);
        points2->savA(2, 1, 150);
        points2->savA(2, 2, 100);

        points2->savA(3, 0, 350);
        points2->savA(3, 1, 150);
        points2->savA(3, 2, 100);

        points2->savA(4, 0, 350);
        points2->savA(4, 1, 110);
        points2->savA(4, 2, 300);

        points2->savA(5, 0, 450);
        points2->savA(5, 1, 110);
        points2->savA(5, 2, 300);

        points2->savA(6, 0, 450);
        points2->savA(6, 1, 160);
        points2->savA(6, 2, 300);

        points2->savA(7, 0, 350);
        points2->savA(7, 1, 160);
        points2->savA(7, 2, 300);

        HMENU MainMenu = CreateMenu();
        HMENU PopupMenu = CreatePopupMenu();

        AppendMenu(MainMenu, MF_STRING | MF_POPUP, (UINT)PopupMenu, L"&Файл");
        {
            AppendMenu(PopupMenu, MF_STRING, IDM_EXIT, L"Выход");
        }
        AppendMenu(MainMenu, MF_STRING, ID_CREATES, L"&Создать");
        AppendMenu(MainMenu, MF_STRING, IDM_ABOUT, L"&О программе");
        SetMenu(hWnd, MainMenu);

        break;
    }
    case WM_KEYDOWN: {
        switch (wParam) {
        case VK_LEFT: {  /* Обрабатывает клавишу LEFT ARROW (Стрелка влево). */
            for (int i = 0; i < points1->sizes(); i++) {
                points1->savA(i, 0, points1->readA(i, 0)- MOVEMENT);
                points2->savA(i, 0, points2->readA(i, 0) - MOVEMENT);
            }
            
            break;
        }
        case VK_RIGHT: {/* Обрабатывает клавишу RIGHT ARROW (Стрелка вправо). */
            for (int i = 0; i < points1->sizes(); i++) {
                points1->savA(i, 0, points1->readA(i, 0) + MOVEMENT);
                points2->savA(i, 0, points2->readA(i, 0) + MOVEMENT);
            }
           
            break;
        }
        case VK_UP: {/* Обрабатывает клавишу UP ARROW (Стрелка вверх). */
            for (int i = 0; i < points1->sizes(); i++) {
                points1->savA(i, 1, points1->readA(i, 1) - MOVEMENT);
                points2->savA(i, 1, points2->readA(i, 1) - MOVEMENT);
            }
            
            break;
        }
        case VK_DOWN: {/* Обрабатывает клавишу DOWN ARROW (Стрелка вниз). */
            for (int i = 0; i < points1->sizes(); i++) {
                points1->savA(i, 1, points1->readA(i, 1) + MOVEMENT);
                points2->savA(i, 1, points2->readA(i, 1) + MOVEMENT);
            }
            break;
        }
        default:
            break; /* Обрабатывает другие не символьные нажатия клавиш. */
        }
    }
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case ID_CREATES: {
           
            break;
        }
        case IDM_ABOUT: {
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        }
        case IDM_EXIT: {
            //point.~PointBuffer();
            //point = NULL;
            DestroyWindow(hWnd);
            break;
        }
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            frameBuffer.Clear({ 255,255,255 });
            int j;
            for (int i = 0; i < points1->sizes() / 2; i++) {
                j = i + 1;
                if (j == points1->sizes() / 2) {
                    j = 0;
                }
                points1->SetLine3D(&frameBuffer, camera, i, j, { 0,255,0 });
                points2->SetLine3D(&frameBuffer, camera, i, j, { 0,255,0 });
            }
            for (int i = points1->sizes() / 2; i < points1->sizes(); i++) {
                j = i + 1;
                if (j == points1->sizes()) {
                    j = points1->sizes() / 2;
                }
                points1->SetLine3D(&frameBuffer, camera, i, j, { 0,0,255 });
                points2->SetLine3D(&frameBuffer, camera, i, j, { 0,0,255 });
            }
            for (int i = 0; i < points1->sizes() / 2; i++) {
                j = i + 4;
                points1->SetLine3D(&frameBuffer, camera, i, j, { 255,0,255 });
                points2->SetLine3D(&frameBuffer, camera, i, j, { 255,0,255 });
            }
            int ind[6][4] = { {0,1,2,3},
                {4,5,6,7},
                {0,1,5,4},
                {0,4,7,3},
                {3,7,6,2},
                {2,6,5,1} };
            vector<vector<double>> shadow1;
            vector<vector<double>> shadow2;
            points1->EnterShadow(&frameBuffer, camera, earth, shadow1);
            points2->EnterShadow(&frameBuffer, camera, earth, shadow2);
            if (shadow1.size() > 1) {
                for (int i = 0; i < shadow1.size() - 1; i++) {
                    int f = 0;
                    int min = i;
                    for (int l = 0; l < shadow1.size() - 1 - i; l++) {
                        if (shadow1[l][0] > shadow1[l + 1][0]) {
                            vector<double> S = { shadow1[l][0] , shadow1[l][1] , shadow1[l][2] };
                            shadow1[l][0] = shadow1[l + 1][0];
                            shadow1[l][1] = shadow1[l + 1][1];
                            shadow1[l][2] = shadow1[l + 1][2];
                            shadow1[l + 1][0] = S[0];
                            shadow1[l + 1][1] = S[1];
                            shadow1[l + 1][2] = S[2];
                            f = 1;
                        }
                        if (shadow1[l][0] < shadow1[min][0]) min = l;
                    }
                    if (!f) break;
                    if (min != i) {
                        vector<double> S = { shadow1[i][0] , shadow1[i][1] , shadow1[i][2] };
                        shadow1[i][0] = shadow1[min][0];
                        shadow1[i][1] = shadow1[min][1];
                        shadow1[i][2] = shadow1[min][2];
                        shadow1[min][0] = S[0];
                        shadow1[min][1] = S[1];
                        shadow1[min][2] = S[2];
                    }
                }
                int k = 0;
                for (int i = 0; i < shadow1.size(); i++) {
                    k = i + 1;
                    if (k == shadow1.size()) {
                        k = i;
                    }
                    else {

                    }
                    SetLine3D(&frameBuffer, shadow1, i, k, { 128,128,128 });
                }
            }
            if (shadow2.size() > 1) {
                for (int i = 0; i < shadow2.size() - 1; i++) {
                    int f = 0;
                    int min = i;
                    for (int l = 0; l < shadow2.size() - 1 - i; l++) {
                        if (shadow2[l][0] > shadow2[l + 1][0]) {
                            vector<double> S = { shadow2[l][0] , shadow2[l][1] , shadow2[l][2] };
                            shadow2[l][0] = shadow2[l + 1][0];
                            shadow2[l][1] = shadow2[l + 1][1];
                            shadow2[l][2] = shadow2[l + 1][2];
                            shadow2[l + 1][0] = S[0];
                            shadow2[l + 1][1] = S[1];
                            shadow2[l + 1][2] = S[2];
                            f = 1;
                        }
                        if (shadow2[l][0] < shadow2[min][0]) min = l;
                    }
                    if (!f) break;
                    if (min != i) {
                        vector<double> S = { shadow2[i][0] , shadow2[i][1] , shadow2[i][2] };
                        shadow2[i][0] = shadow2[min][0];
                        shadow2[i][1] = shadow2[min][1];
                        shadow2[i][2] = shadow2[min][2];
                        shadow2[min][0] = S[0];
                        shadow2[min][1] = S[1];
                        shadow2[min][2] = S[2];
                    }
                }
                int k = 0;
                for (int i = 0; i < shadow2.size(); i++) {
                    k = i + 1;
                    if (k == shadow2.size()) {
                        k = i;
                    }
                    SetLine3D(&frameBuffer, shadow2, i, k, { 128,128,128 });
                }
            }

            for (int i = 0; i < 6; i++) {
                points1->EnterSketch(&frameBuffer, frameBuffer.GetWidth(), frameBuffer.GetHeight(), (int*)ind[i], { 255,255,255 }, { 120,150,100 });
                points2->EnterSketch(&frameBuffer, frameBuffer.GetWidth(), frameBuffer.GetHeight(), (int*)ind[i], { 255,255,255 }, { 42,100,150 });
            }
            PresentFrame(frameBuffer.GetWidth(), frameBuffer.GetHeight(), frameBuffer.GetData(), hWnd);
            EndPaint(hWnd, &ps);
        
       
        break;
    }
    case WM_DESTROY:
       
        PostQuitMessage(0);

        break;
    case WM_SIZE: {
        RECT rect;
        RECT mouse;
        GetClientRect(hWnd, &rect);
        int x = rect.right / 2;
        int y = rect.bottom / 2;
        mouse.right = x;
        mouse.left = x;
        mouse.bottom = y;
        mouse.top = y;
        SetCursorPos(x, y);
        ClipCursor(&mouse);
        earth[0].coordEarth.x = 0;
        earth[0].coordEarth.y = 0.7* rect.bottom;
        earth[0].coordEarth.z = -1000;

        earth[1].coordEarth.x = rect.right;
        earth[1].coordEarth.y = 0.7* rect.bottom;
        earth[1].coordEarth.z = -1000;

        earth[2].coordEarth.x = rect.right;
        earth[2].coordEarth.y = rect.bottom;
        earth[2].coordEarth.z = 1000;

        earth[3].coordEarth.x = 0;
        earth[3].coordEarth.y = rect.bottom;
        earth[3].coordEarth.z = 1000;
        camera.coordLight.x = rect.right / 3;
        camera.coordLight.y = rect.bottom * 10;
        camera.coordLight.z = 10;
        frameBuffer = BitmapBuffer(rect.right, rect.bottom, { 0, 0, 0 });
        camera.SetCoordCam(rect.right, rect.bottom);

        break;
    }
    default: {
        return DefWindowProc(hWnd, message, wParam, lParam);

    }
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR  CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}