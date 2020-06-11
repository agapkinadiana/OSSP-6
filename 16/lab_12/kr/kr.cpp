// kr.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>


#include <windows.h>

#include <wchar.h>

#include <time.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void DrawPixels(HWND hwnd);

int WINAPI wWinMain(

	HINSTANCE hInstance,

	HINSTANCE hPrevInstance,

	PWSTR lpCmdLine,

	int nCmdShow

) {

	MSG msg;

	WNDCLASSW wc = { 0 };

	wc.style = CS_HREDRAW | CS_VREDRAW;

	wc.lpszClassName = L"Floor windows in green";

	wc.hInstance = hInstance;

	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);

	wc.lpfnWndProc = WndProc;

	wc.hCursor = LoadCursor(0, IDC_ARROW);

	RegisterClassW(&wc);

	CreateWindowW(wc.lpszClassName, L"Floor windows in green",

		WS_OVERLAPPEDWINDOW | WS_VISIBLE,

		0, 0, 300, 250, NULL, NULL, hInstance, NULL);

	while (GetMessage(&msg, NULL, 0, 0)) {

		TranslateMessage(&msg);

		DispatchMessage(&msg);

	}

	srand(time(NULL));

	return (int)msg.wParam;

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg,

	WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_PAINT:

		DrawPixels(hwnd);

		break;

	case WM_DESTROY:

		PostQuitMessage(0);

		return 0;

	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);

}

void DrawPixels(HWND hwnd) {

	PAINTSTRUCT ps;

	RECT r;

	GetClientRect(hwnd, &r);

	if (r.bottom == 0) {

		return;

	}

	HDC hdc = BeginPaint(hwnd, &ps);

	for (int x = 150; x <= 300; x++) {
		for (int y = 0; y <= 250; y++) {

			SetPixel(hdc, x, y, RGB(0, 255, 0));
		}
	}

	EndPaint(hwnd, &ps);

}