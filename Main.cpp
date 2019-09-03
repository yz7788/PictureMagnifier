//*****************************************************************************
//
// Main.cpp : Defines the entry point for the application.
// Creates a white RGB image with a black line and displays it.
// Two images are displayed on the screen.
// Left Pane: Input Image, Right Pane: Modified Image
//
// Author - Parag Havaldar
// Code used by students as a starter code to display and modify images
//
//*****************************************************************************

// Include class files
#include "Image.h"
#include <string.h>
#include <iostream>
#include <windows.h>

using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
MyImage			inImage; // image objects
HINSTANCE		hInst;							// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// The title bar text
bool flag = FALSE;

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void				 NewPixelValue_nearestneighbour(char* RBuf, char* GBuf, char* BBuf, float x, float y, int w, int h, char* newData, int index);
void				 NewPixelValue_bilinear(char* RBuf, char* GBuf, char* BBuf, float x, float y, int w, int h, char* newData, int index);



// Main entry point for a windows application
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	// Create a separate console window to display output to stdout
	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	// read argument (mode, scale, antiliasing)
	if (lpCmdLine[0] == 0) {
		wprintf(L"No command line arguement.");
		return -1;
	}
	int cnt = 0;
	string str = lpCmdLine;
	while (str[cnt] != ' ' && str[cnt] != 0) {
		cnt++;
	}
	lpCmdLine[cnt] = 0;
	cout << "The first parameter was: " << lpCmdLine << endl;
	int mode = str[cnt + 1] - '0';

	string scale_str = str.substr(cnt + 3, str.length());
	float scale = atof(scale_str.c_str());
	
	cnt = cnt + 3;
	while (str[cnt] != ' ' && str[cnt] != 0) {
		cnt++;
	}
	int antialiasing = str[cnt + 1] - '0';
	inImage.setMode(mode);


	// Set up the images
	int w = 1920;
	int h = 1080;
	inImage.setWidth(w);
	inImage.setHeight(h);
	inImage.setScale(scale);
	inImage.setAA(antialiasing);

	inImage.setImagePath(lpCmdLine);
	inImage.ReadImage();


	// Set up RGBbuf
	char* RBuf = new char[w * h];
	char* GBuf = new char[w * h];
	char* BBuf = new char[w * h];

	for (int i = 0; i < w * h; i++) {
		RBuf[i] = *(inImage.getImageData() + 3 * i);
		GBuf[i] = *(inImage.getImageData() + 3 * i + 1);
		BBuf[i] = *(inImage.getImageData() + 3 * i + 2);
	}

	/*
	POINT pt;
	GetCursorPos(&pt);
	inImage.setPT(pt);
	inImage.ZoomOut();
	*/

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_IMAGE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_IMAGE);
	WPARAM wParam;
	LPARAM lParam;
	RECT rt;
	

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			
			if (mode == 2) {
				POINT pt;
				GetCursorPos(&pt);
				Sleep(20);
				CRect rect;
				GetWindowRect(msg.hwnd,&rect);
				pt.x = pt.x - rect.left;
				pt.y = pt.y - rect.top - 150;
				if (50 < pt.x < w-50 && 50 < pt.y < h-50) {
					inImage.setPT(pt);
					inImage.ZoomOut();
					GetClientRect(msg.hwnd, &rt);
					InvalidateRect(msg.hwnd, &rt, false);
				}
			}
				
			}
		}
	return msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_IMAGE);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = (LPCSTR)IDC_IMAGE;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}


//
//   FUNCTION: InitInstance(HANDLE, int)
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
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}


//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// TO DO: part useful to render video frames, may place your own code here in this function
	// You are free to change the following code in any way in order to display the video

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	RECT rt;
	GetClientRect(hWnd, &rt);

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case ID_MODIFY_IMAGE:
			if (inImage.getMode() == 1) {
				if (flag == FALSE) {
					inImage.Modify();
					InvalidateRect(hWnd, &rt, true);
					flag = TRUE;
				}
			}
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		// TO DO: Add any drawing code here...
		char text[1000];
		strcpy(text, "Mode 1: Click File -> Modify. Repeated press of Modify is Useless. \n");
		DrawText(hdc, text, strlen(text), &rt, DT_LEFT);
		strcpy(text, "\nMode 2: Move the mouse on the picture. \n");
		DrawText(hdc, text, strlen(text), &rt, DT_LEFT);

		
		BITMAPINFO bmi;
		CBitmap bitmap;
		memset(&bmi, 0, sizeof(bmi));
		bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
		bmi.bmiHeader.biWidth = inImage.getWidth();
		bmi.bmiHeader.biHeight = -inImage.getHeight();  // Use negative height.  DIB is top-down.
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 24;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biSizeImage = inImage.getWidth() * (inImage.getHeight());

		SetDIBitsToDevice(hdc,
			0, 100, inImage.getWidth(), inImage.getHeight(),
			0, 0, 0, inImage.getHeight(),
			inImage.getImageData(), &bmi, DIB_RGB_COLORS);

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


// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

