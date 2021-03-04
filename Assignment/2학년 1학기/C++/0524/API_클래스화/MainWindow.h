#pragma once

#include <Windows.h>
#include <iostream>
using namespace std;

class MainWindow {
private:
	static MainWindow* pthis;
	MainWindow();
	~MainWindow();

	HWND m_hWnd;
public:
	static MainWindow* GetInstance();
	static void Destory();

	BOOL Begin(LPCTSTR, HINSTANCE, INT, INT, INT);
	INT MessageLoop();
	LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);
};