// spaceship.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "spaceship.h"
#include <cmath>

#define M_PI 3.141596

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SPACESHIP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPACESHIP));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPACESHIP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SPACESHIP);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

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
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

RECT rc;
Spaceship ship;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GetClientRect(hWnd, &rc);
	Spaceship ship(&rc);

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case IDM_SPIN_SHIP_CW:
				ship.spinCW();
				break;
			case IDM_SPIN_SHIP_CCW:
				ship.spinCCW();
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			
			ship.updateShip();
			ship.drawShip(hdc);
			
			InvalidateRect(hWnd, &rc, TRUE);
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

// Message handler for about box.
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



void Spaceship::drawShip(HDC hdc)
{
	MoveToEx(hdc, leftWingX, leftWingY, NULL);
	LineTo(hdc, noseX, noseY);
	MoveToEx(hdc, noseX, noseY, NULL);
	LineTo(hdc, rightWingX, rightWingY);
	MoveToEx(hdc, rightWingX, rightWingY, NULL);
	LineTo(hdc, midX, midY);
	MoveToEx(hdc, midX, midY, NULL);
	LineTo(hdc, leftWingX, leftWingY);
}



void Spaceship::updateShip()
{
	mainX += speedX * dirX;
	mainY += speedY * dirY;

	leftWingX = mainX - sqrt(41) * cos(angle - 38.6598);
	leftWingY = mainY - sqrt(41) * sin(angle - 38.6598);

	rightWingX = mainX + sqrt(41) * cos(141.34019);
	rightWingY = mainY - sqrt(41) * sin(141.34019);

	noseX = mainX + 4 * cos(angle);
	noseY = mainY + 4 * sin(angle);

	midX = mainX - 2 * cos(angle);
	midY = mainY - 2 * sin(angle);
}



void Spaceship::updateShip(int scale)
{
	mainX += speedX * dirX * scale;
	mainY += speedY * dirY;

	leftWingX = mainX - sqrt(41) * cos(angle - 38.6598) * scale;
	leftWingY = mainY - sqrt(41) * sin(angle - 38.6598) * scale;

	rightWingX = mainX + sqrt(41) * cos(141.34019) * scale;
	rightWingY = mainY - sqrt(41) * sin(141.34019) * scale;

	noseX = mainX + 4 * cos(angle) * scale;
	noseY = mainY + 4 * sin(angle) * scale;

	midX = mainX - 2 * cos(angle) * scale;
	midY = mainY - 2 * sin(angle) * scale;
}



double Spaceship::shipAngle()
{
	if (speedX == 0)					return 90;
	else if (speedX > 0 && speedY > 0)	return (180/M_PI)*atan(speedX / speedY);
	else if (speedX < 0 && speedY > 0)	return 180 + (180 / M_PI)*atan(speedX / speedY);
	else if (speedX < 0 && speedY < 0)	return 180 + (180 / M_PI)*atan(speedX / speedY);
	else if (speedX > 0 && speedY < 0)	return 360 + (180 / M_PI)*atan(speedX / speedY);
}

Spaceship::Spaceship(RECT *rect)
{
	angle = 90;
	mainX = rc.right / 2;
	mainY = rc.bottom / 2;
	speedX = 0;
	speedY = 0;
	dirX = 1;
	dirY = 1;
}

Spaceship::Spaceship()
{
	angle = 90;
	mainX = 200;
	mainY = 200;
	speedX = 0;
	speedY = 0;
	dirX = 1;
	dirY = 1;
}