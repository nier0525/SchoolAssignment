#include "MainWindow.h"

// �̱��� ó��

MainWindow* MainWindow::pthis = 0;

MainWindow* MainWindow::GetInstance() {
	if (!pthis) {
		pthis = new MainWindow();
	}
	return pthis;
}

void MainWindow::Destory() {
	if (pthis) {
		delete pthis;
		pthis = nullptr;
	}
}

// ������,�Ҹ���

MainWindow::MainWindow() {
	m_hWnd = nullptr;
}

MainWindow::~MainWindow() {

}

// ������ ���ν���

static LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	// ������ ���ν��� �ݹ� �Լ��� �ѱ�
	return MainWindow::GetInstance()->WndProc(hWnd, Msg, wParam, lParam);
}

// ������ ���� �� �ʱ�ȭ

BOOL MainWindow::Begin(LPCTSTR name, HINSTANCE hInstance, INT CmdShow, INT Width, INT Height) {
	// ���� ó��
	if (m_hWnd != nullptr) {
		return false;
	}

	WNDCLASS WndClass;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	WndClass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = ::WndProc;
	WndClass.lpszClassName = name;
	WndClass.lpszMenuName = nullptr;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;

	// ������ Ŭ���� ���
	if (!RegisterClass(&WndClass)) {
		return false;
	}

	// ������ ����
	m_hWnd = CreateWindow(name, name, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr, (HMENU)nullptr, hInstance, nullptr);
	if (!m_hWnd) {
		return false;
	}

	// ������ ����
	if (ShowWindow(m_hWnd, CmdShow)) {
		return false;
	}
	return true;
}

// �޼��� ����

INT MainWindow::MessageLoop() {
	MSG Message = { 0 };

	while (GetMessage(&Message, nullptr, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);		
	}

	return (int)Message.wParam;
}

// ������ ���ν��� �ݹ�

LRESULT MainWindow::WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {

	switch (Msg) {
	
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, Msg, wParam, lParam));
}