#include <windows.h>
#include <iostream>
using namespace std;

#include "Engine.h"
#include "Keyboard.h"
#include "Constants.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
const TCHAR szName[] = TEXT("win");

Engine SilverEngine;

void killWindow(HWND hWnd, HINSTANCE hInstance, WNDCLASS wndClass) {

	SilverEngine.shutDown(hWnd);
	DestroyWindow(hWnd);
	UnregisterClass(wndClass.lpszClassName, hInstance);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
	HWND hWnd;
	MSG msg;
	//WNDCLASS�ṹ��
	WNDCLASS wndClass;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = szName;

	if (!RegisterClass(&wndClass)) {
		MessageBox(NULL, TEXT("Can not create!"), szName, MB_ICONERROR);
		return 0;
	}

	hWnd = CreateWindow(szName, TEXT("Soft Render"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, SCREEN_WIDTH, SCREEN_HEIGHT,
		NULL, NULL, hInstance, NULL);
	SilverEngine.startUp(hWnd);

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);
	willExit = false;
	memset(&msg, 0, sizeof(MSG));

	//ʵʱ�¼���ѯ��ʽ
	while (!willExit) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { //�鿴Ӧ�ó�����Ϣ���У�����Ϣ�ͷ��ͳ�ȥ
			if (msg.message == WM_QUIT)
				willExit = true;
			else {
				TranslateMessage(&msg); //����Ϣת��Ϊ�������Ϣ��������ټ���������
				DispatchMessage(&msg); //����Ϣ������Win���ڣ�����WinProc��һ�������¼���
			}
		}
		else {
			SilverEngine.frame(); //����ͼ��
		}
	}

	killWindow(hWnd, hInstance, wndClass);
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_SIZE:
		SilverEngine.Resize(SCREEN_WIDTH, SCREEN_HEIGHT);

		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_KEYDOWN:
		keyDown(wParam);
		break;
	case WM_KEYUP:
		keyUp(wParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_QUERYNEWPALETTE:
		if (!SilverEngine.engine_displayManager.hPalette)
			break;
		SelectPalette(SilverEngine.engine_displayManager.hdc, SilverEngine.engine_displayManager.hPalette, FALSE);
		RealizePalette(SilverEngine.engine_displayManager.hdc);
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_PALETTECHANGED:
		if (!SilverEngine.engine_displayManager.hPalette || (HWND)wParam == hWnd)
			break;
		SelectPalette(SilverEngine.engine_displayManager.hdc, SilverEngine.engine_displayManager.hPalette, FALSE);
		RealizePalette(SilverEngine.engine_displayManager.hdc);
		UpdateColors(SilverEngine.engine_displayManager.hdc);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
