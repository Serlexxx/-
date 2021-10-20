// Заготовка КГ.cpp : Определяет точку входа для приложения.
//
using namespace std;
#include <iostream>
#include "framework.h"
#include "Resource.h"

#include "Painter.h"
#include "Point.h"
#include "Camera.h"

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
//INT_PTR  CALLBACK    Creates(HWND, UINT, WPARAM, LPARAM);

//Добавление отрисовщика



// Размеры кадра и указатель на массив структур RGBQUAD (буфер кадра)
uint32_t frameWidth, frameHeight;
RGBQUAD* frameBuffer;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.


		

	
    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, NULL, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY, NULL, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
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

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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


Point point;
Camera camera;

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
    case WM_CREATE: {
        HMENU MainMenu = CreateMenu();
        HMENU PopupMenu = CreatePopupMenu();

        AppendMenu(MainMenu, MF_STRING | MF_POPUP, (UINT)PopupMenu, L"&Файл");
        {
            AppendMenu(PopupMenu, MF_STRING, IDM_EXIT, L"Выход");
        }
        AppendMenu(MainMenu, MF_STRING, ID_CREATES, L"&Создать");
        AppendMenu(MainMenu, MF_STRING, IDM_ABOUT, L"&О программе");
        SetMenu(hWnd, MainMenu);
        }

        break;
    case WM_KEYDOWN: {
        switch (wParam) {
            case VK_LEFT:  /* Обрабатывает клавишу LEFT ARROW (Стрелка влево). */
                for (int i = 0; i < point.SetApex(); i++) {
                    point.pointer[i].x -= MOVEMENT;
                }
                SendMessage(hWnd, WM_PAINT, NULL, NULL);
                InvalidateRect(hWnd, 0, true);
            break;

            case VK_RIGHT: /* Обрабатывает клавишу RIGHT ARROW (Стрелка вправо). */
                for (int i = 0; i < point.SetApex(); i++) {
                    point.pointer[i].x += MOVEMENT;
                }
                SendMessage(hWnd, WM_PAINT, NULL, NULL);
                InvalidateRect(hWnd, 0, true);
            break;

            case VK_UP: /* Обрабатывает клавишу UP ARROW (Стрелка вверх). */
                for (int i = 0; i < point.SetApex(); i++) {
                    point.pointer[i].y -= MOVEMENT;
                }
                SendMessage(hWnd, WM_PAINT, NULL, NULL);
                InvalidateRect(hWnd, 0, true);
            break;

            case VK_DOWN: /* Обрабатывает клавишу DOWN ARROW (Стрелка вниз). */
                for (int i = 0; i < point.SetApex(); i++) {
                    point.pointer[i].y += MOVEMENT;
                }
                SendMessage(hWnd, WM_PAINT, NULL, NULL);
                InvalidateRect(hWnd, 0, true);
            break;

            case VK_HOME: /* Обрабатывает клавишу HOME. */ 
            {
               

                break;
            }
            case VK_END: /* Обрабатывает клавишу END. */

            break;

            case VK_INSERT: /* Обрабатывает клавишу INS. */

            break;

            case VK_DELETE: /* Обрабатывает клавишу DEL. */

            break;

            case VK_F1: /* Обрабатывает клавишу F2. */
            {
                
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
            case ID_CREATES:
                point.GetApex();
                point.GetPoint();

                //установка координат камеры относительно центра окна
                camera.SetCoordCam(frameHeight, frameWidth);

                SendMessage(hWnd, WM_PAINT, NULL, NULL);
                InvalidateRect(hWnd, 0, true);
                break;

            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;

            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
			frameWidth = ps.rcPaint.right;
			frameHeight = ps.rcPaint.bottom;
            
			// Создать буфер кадра по размерам клиенсткой области
            frameBuffer = CreateFrameBuffer(frameWidth, frameHeight, { 255,255,255,0 });

            

			// Рисование линии
            point.CentralProjection(camera);
            SetLine(frameBuffer, frameWidth, point, { 0,0,0,0 });
            SetLine_pr(frameBuffer, frameWidth, point, { 100,100,100,0 });
            PresentFrame(hdc, frameWidth, frameHeight, frameBuffer, hWnd);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
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

// Обработчик сообщений для окна "Создать".
/*
INT_PTR CALLBACK Creates(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
*/





