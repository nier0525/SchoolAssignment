#include "MainWindow.h"

// 싱글톤 처리

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

// 생성자,소멸자

MainWindow::MainWindow() {
	m_hWnd = nullptr;
}

MainWindow::~MainWindow() {

}

// 윈도우 프로시저

static LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	// 윈도우 프로시저 콜백 함수로 넘김
	return MainWindow::GetInstance()->WndProc(hWnd, Msg, wParam, lParam);
}

// 윈도우 생성 및 초기화

BOOL MainWindow::Begin(LPCTSTR name, HINSTANCE hInstance, INT CmdShow, INT Width, INT Height) {
	// 에러 처리
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

	// 윈도우 클래스 등록
	if (!RegisterClass(&WndClass)) {
		return false;
	}

	// 윈도우 생성
	m_hWnd = CreateWindow(name, name, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr, (HMENU)nullptr, hInstance, nullptr);
	if (!m_hWnd) {
		return false;
	}

	// 윈도우 보기
	if (ShowWindow(m_hWnd, CmdShow)) {
		return false;
	}
	return true;
}

// 메세지 루프

INT MainWindow::MessageLoop() {
	MSG Message = { 0 };

	while (GetMessage(&Message, nullptr, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);		
	}

	return (int)Message.wParam;
}

// 윈도우 프로시저 콜백

LRESULT MainWindow::WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {

	switch (Msg) {
	
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, Msg, wParam, lParam));
}