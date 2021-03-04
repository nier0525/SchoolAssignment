#include "MainWindow.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
	if (MainWindow::GetInstance()->Begin(TEXT("First"), hInstance, nCmdShow, CW_USEDEFAULT, CW_USEDEFAULT)) {
		return MainWindow::GetInstance()->MessageLoop();
	}
	MainWindow::Destory();
}