#include "WindowFrame.h"

CWindowFrame* CWindowFrame::mPthis=NULL;

CWindowFrame* CWindowFrame::Create(HINSTANCE _hinstance)
{
	if(!mPthis)
	{
		mPthis=new CWindowFrame();
	}

	CWindowFrame::GetInstance()->mhInstance=_hinstance;

	return mPthis;
}

CWindowFrame* CWindowFrame::GetInstance()
{	
	return mPthis;
}

void CWindowFrame::Destroy()
{
	if(mPthis)
	{
		delete mPthis;
		mPthis=NULL;
	}
}

LRESULT CALLBACK CWindowFrame::WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;

	static int x, y;
	static bool Draw = false;

	switch (iMessage) {

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_RBUTTONDOWN:
		InvalidateRect(hWnd, nullptr, true);
		break;

	case WM_LBUTTONDOWN:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		Draw = true;
		break;

	case WM_LBUTTONUP:
		hdc = GetDC(hWnd);
		MoveToEx(hdc, x, y, nullptr);
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		LineTo(hdc, x, y);
		Draw = false;
		ReleaseDC(hWnd, hdc);
		break;

	case WM_MOUSEMOVE:
		if (Draw) {
			hdc = GetDC(hWnd);
			MoveToEx(hdc, x, y, nullptr);
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			LineTo(hdc, x, y);
			ReleaseDC(hWnd, hdc);
		}
		break;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

int CWindowFrame::Run(MSG _msg)
{
	TranslateMessage(&_msg);
	DispatchMessage(&_msg);

	return _msg.wParam;
}

void CWindowFrame::Initialize()
{
}

void CWindowFrame::BuildWindow()
{	
	WNDCLASS WndClass;
	
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance=CWindowFrame::GetInstance()->mhInstance;
	WndClass.lpfnWndProc=CWindowFrame::GetInstance()->WndProc;
	WndClass.lpszClassName=TEXT("exam");
	WndClass.lpszMenuName=NULL;
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	mhWnd=CreateWindow(TEXT("exam"),TEXT("exam"),WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,(HMENU)NULL,mhInstance,NULL);

	ShowWindow(mhWnd, SW_SHOW);
}