#include "WindowFrame.h"
#include "Mouse.h"

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
	static HDC hdc;
	static HDC backDC, DBDC;
	static PAINTSTRUCT ps;
	static RECT rt;
	static HBITMAP DB, oldDB;	
	
	Mouse* mouse = Mouse::GetInstance();

	static int x, y;

	switch (iMessage) {

	case WM_DESTROY:
		SelectObject(backDC, oldDB);
		DeleteObject(DB);
		DeleteDC(backDC);
		PostQuitMessage(0);
		return 0;

	case WM_SIZE:
		GetClientRect(hWnd, &rt);
		break;

	case WM_CREATE:
		mouse->Init();
		{
			GetClientRect(hWnd, &rt);
			hdc = GetDC(hWnd);
			backDC = CreateCompatibleDC(hdc);
			DB = CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
			oldDB = (HBITMAP)SelectObject(backDC, DB);
			PatBlt(backDC, 0, 0, rt.right, rt.bottom, WHITENESS);
			ReleaseDC(hWnd, hdc);
		}
		break;

	case WM_RBUTTONDOWN:
		GetClientRect(hWnd, &rt);
		hdc = GetDC(hWnd);
		PatBlt(backDC, 0, 0, rt.right, rt.bottom, WHITENESS);
		ReleaseDC(hWnd, hdc);
		break;

	case WM_LBUTTONDOWN:
		mouse->SetLbuttonDown();
		mouse->SetPos(lParam);
		break;

	case WM_LBUTTONUP:
		mouse->SetLbuttonUp();
		break;

	case WM_MOUSEMOVE:
		if (Mouse::GetInstance()->GetLbuttonState()) {
			mouse->SetPos(lParam);
			InvalidateRect(hWnd, nullptr, false);
		}
		break;

	case WM_PAINT:
		GetClientRect(hWnd, &rt);
		hdc = BeginPaint(hWnd,&ps);

		MoveToEx(backDC, mouse->GetOldPosX(), mouse->GetOldPosY(), nullptr);
		LineTo(backDC, mouse->GetPosX(), mouse->GetPosY());

		BitBlt(hdc, 0, 0, rt.right, rt.bottom, backDC, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
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