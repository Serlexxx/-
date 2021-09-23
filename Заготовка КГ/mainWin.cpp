// Заготовка КГ.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Painter.h"
#include "Resource.h"
#include "Point.h"

#define MAX_LOADSTRING 100
#define ID_CREATE 160
#define ID_FIRSTCHILD	161
// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING] = L"Компьютерная графика";;                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING] = L"Компьютерная графика";            // имя класса главного окна


// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Create(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildProc(HWND, UINT, WPARAM, LPARAM);
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
        AppendMenu(MainMenu, MF_STRING, ID_CREATE, L"&Создать");
        AppendMenu(MainMenu, MF_STRING, IDM_ABOUT, L"&О программе");
        SetMenu(hWnd, MainMenu);

        

        }

        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case ID_CREATE:

                DialogBoxW(hInst, MAKEINTRESOURCE(IDR_MAINFRAME), hWnd, Create);
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
			frameBuffer = CreateFrameBuffer(frameWidth, frameHeight);
			

			#define M_PI 3.14159265358979323846
			double cnt = 5; //Коеффициент перемещения
			double coef1 = 1.05; //Коеффициент увеличения
			double coef2 = 0.95; //Коеффициент уменьшения
			double coef3 = 10 * M_PI / 180; //Коеффициент вращения +
			double coef4 = -10 * M_PI / 180; //Коеффициент вращения -





			// Рисование линии
			
			//V_FP1(frameBuffer, frameWidth, &pointer[0].x, { 0,255,0,0 });

            // Мои добавления
            PresentFrame(hdc, frameWidth, frameHeight, frameBuffer, hWnd);
            SetLine(frameBuffer, frameWidth, { 0,0,255,0 });
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
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
INT_PTR CALLBACK Create(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);

    switch (message)
    {
    case WM_INITDIALOG:
       
        return (INT_PTR)TRUE;
    case WM_CREATE:
        /*
        WNDCLASS w;
        memset(&w, 0, sizeof(WNDCLASS));
        w.lpfnWndProc = ChildProc;
        w.hInstance = hInst;
        w.hbrBackground = WHITE_BRUSH;
        w.lpszClassName = L"&ChildWClass";
        w.hCursor = LoadCursor(NULL, IDC_CROSS);
        RegisterClass(&w);
        HWND child;
        child = CreateWindowEx(0, L"&ChildWClass", (LPCTSTR)NULL,
            WS_CHILD | WS_BORDER | WS_VISIBLE, 10, 10,
            50, 50, hDlg, (HMENU)(int)(ID_FIRSTCHILD), hInst, NULL);
        ShowWindow(child, SW_NORMAL);
        UpdateWindow(child);
        EndDialog(hDlg, LOWORD(wParam));
        */
        break;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            
            return (INT_PTR)TRUE;
        }
        
        break;
    }
    return (INT_PTR)FALSE;
}

LRESULT CALLBACK ChildProc(HWND hwnd, UINT Message, WPARAM wparam, LPARAM lparam)
{
    if (Message == WM_DESTROY)
    {
        return 0;
    }
    return DefWindowProc(hwnd, Message, wparam, lparam);
}

/**
 * \brief Отрисовка кадра
 * \param width Ширина
 * \param height Высота
 * \param pixels Массив пикселов
 * \param hWnd Хендл окна, device context которого будет использован
 */
void PresentFrame( HDC hdc, uint32_t width, uint32_t height, void* pixels, HWND hWnd)
{
	// Получить хендл на временный bit-map (4 байта на пиксель)
	HBITMAP hBitMap = CreateBitmap(width, height, 1, 8 * 4, pixels);

	// Временный DC для переноса bit-map'а
	HDC srcHdc = CreateCompatibleDC(hdc);

	// Связать bit-map с временным DC
	SelectObject(srcHdc, hBitMap);

	// Копировать содержимое временного DC в DC окна
	BitBlt(
		hdc,    // HDC назначения
		0,      // Начало вставки по оси X
		0,      // Начало вставки по оси Y
		width,  // Ширина
		height, // Высота
		srcHdc, // Исходный HDC (из которого будут копироваться данные)
		0,      // Начало считывания по оси X
		0,      // Начало считывания по оси Y
		SRCCOPY // Копировать
	);
	//Sleep(10000);
	// Уничтожить bit-map
	DeleteObject(hBitMap);
	// Уничтодить временный DC
	DeleteDC(srcHdc);
	// Уничтодить DC
	//DeleteDC(hdc);
	ReleaseDC(hWnd, hdc);
	//PostQuitMessage(0);
}



