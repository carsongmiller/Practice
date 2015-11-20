// Accelerating Circle.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Accelerating Circle.h"
#include <cmath>
#include <sstream>
#include <string>

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
    LoadStringW(hInstance, IDC_ACCELERATINGCIRCLE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ACCELERATINGCIRCLE));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ACCELERATINGCIRCLE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ACCELERATINGCIRCLE);
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

#define NUM_ATOM 100
#define ATOM_SIZE 10

double xSpeed[NUM_ATOM];
double ySpeed[NUM_ATOM];
double xPos[NUM_ATOM];
double yPos[NUM_ATOM];
int LR[NUM_ATOM]; //false = moving right, true = moving left
int UD[NUM_ATOM]; //false = moving down, true = moving up
HBRUSH brush[NUM_ATOM];
HRGN rgn[NUM_ATOM];
RECT rc;
RECT tempBox, pressureBox;
double accelFactor = 2;
double temp;
std::ostringstream tempToString;
std::string tempString;
double pressure;
std::ostringstream pressureToString;
std::string pressureString;
std::wstring stemp;
bool init = true;

//paint buffer variables
HBITMAP buffer = NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
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
			case IDM_COOL_DOWN:
				
				break;
			case IDM_HEAT_UP:
				
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
			GetClientRect(hWnd, &rc);

			pressure = 0;
			temp = 0;

			if (init)
			{
				for (int i = 0; i < NUM_ATOM; i++)
				{
					if (rand() % 2 == 0) LR[i] = 1;
					else LR[i] = -1;
					if (rand() % 2 == 0) UD[i] = 1;
					else UD[i] = -1;
					xSpeed[i] = 2 * ((double)rand() / RAND_MAX) + 1;
					ySpeed[i] = 2 * ((double)rand() / RAND_MAX) + 1;
					xPos[i] = (rc.right - rc.left) * ((double)rand() / RAND_MAX) + rc.left;
					yPos[i] = (rc.bottom - rc.top) * ((double)rand() / RAND_MAX) + rc.top;
					brush[i] = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
				}

				tempBox.left = 5;
				tempBox.top = 5;
				tempBox.bottom = 20;
				tempBox.right = 100;
				pressureBox.left = 5;
				pressureBox.top = 20;
				pressureBox.bottom = 35;
				pressureBox.right = 100;
			}

			for (int i = 0; i < NUM_ATOM; i++)
			{
				Ellipse(hdc, xPos[i], yPos[i], ATOM_SIZE + xPos[i], ATOM_SIZE + yPos[i]);

				if (xPos[i] + ATOM_SIZE >= rc.right)
				{
					LR[i] = -1;
					pressure++;
				}
				else if (xPos[i] <= rc.left)
				{
					LR[i] = 1;
					pressure++;
				}
				if (yPos[i] + ATOM_SIZE >= rc.bottom)
				{
					UD[i] = -1;
					pressure++;
				}
				else if (yPos[i] <= rc.top)
				{
					UD[i] = 1;
					pressure++;
				}

				xPos[i] += xSpeed[i] * LR[i];
				yPos[i] += ySpeed[i] * UD[i];
			}

			for (int i = 0; i < NUM_ATOM; i++)
				temp += sqrt(pow(xSpeed[i], 2) + pow(ySpeed[i], 2));

			temp /= NUM_ATOM;

			tempToString << temp;
			pressureToString << pressure;

			DrawText(hdc, std::to_wstring(temp).c_str(), -1, &tempBox, DT_SINGLELINE | DT_TOP | DT_LEFT);
			DrawText(hdc, std::to_wstring(pressure).c_str(), -1, &pressureBox, DT_SINGLELINE | DT_TOP | DT_LEFT);


			Sleep(10);
			InvalidateRect(hWnd, &rc, TRUE);
			init = false;
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
