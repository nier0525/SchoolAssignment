#include "WindowFrame.h"
#include "BackBit.h"
#include "Mouse.h"
#include "DragDrawing.h"

CWindowFrame* CWindowFrame::mPthis=NULL;

CWindowFrame::CWindowFrame()
{	
	mBackbit = nullptr;
	mHinstance = nullptr;
	mMenu = nullptr;
	mHWnd = nullptr;
}

CWindowFrame* CWindowFrame::Create(HINSTANCE _hinstance)
{
	if(!mPthis)
	{
		mPthis=new CWindowFrame();
	}

	CWindowFrame::GetInstance()->mHinstance=_hinstance;

	CMouse::Create();
	CDragFreeLineDrawing::Create();
	CDragCircleDrawing::Create();
	CDragLineDrawing::Create();
	CDragRectangleDrawing::Create();
	return mPthis;
}

CWindowFrame* CWindowFrame::GetInstance()
{	
	return mPthis;
}

void CWindowFrame::Destroy()
{
	CMouse::Destroy();
	CDragFreeLineDrawing::Destroy();
	CDragCircleDrawing::Destroy();
	CDragLineDrawing::Destroy();
	CDragRectangleDrawing::Destroy();

	if(mPthis)
	{
		delete mPthis;
		mPthis=NULL;
	}	
}

LRESULT CALLBACK CWindowFrame::WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;	
	RECT crt;
	
	static bool test;
	static int x, y;

	switch (iMessage) {

	case WM_CREATE:
		CMouse::GetInstance()->Initialize(hWnd);
		GetClientRect(hWnd, &crt);
	    mPthis->mBackbit=new CBackBit(hWnd, crt.right , crt.bottom );
		mPthis->mMenu=new CMenu();
		mPthis->mMenu->MenuInitialize(IDR_MENU1,hWnd);	
		x = 100;
		y = 100;
		SetTimer(hWnd, 0, 10, nullptr);
		return 0;

	case WM_LBUTTONDOWN:
		CMouse::GetInstance()->SetclickDown();	
		CMouse::GetInstance()->SetXY(lParam);
		//if (mPthis->mMenu->IsSelectDraw())
		//{
		//	mPthis->mMenu->SetDrawStartPoint((int)LOWORD(lParam), (int)HIWORD(lParam));
		//}		
		return 0;

	case WM_LBUTTONUP:	
		//if (mPthis->mMenu->IsSelectDraw())
		//{
		//	mPthis->mMenu->Drawing(mPthis->mBackbit, NORMAL);
		//}		
		CMouse::GetInstance()->SetclickUp();
		return 0;

	case WM_RBUTTONDOWN:
		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_SPACE) {
			x++;
			mPthis->mBackbit->ClearBitMap();
		}
		break;

	case WM_COMMAND:
		mPthis->mMenu->ProcessCommandMeg(wParam);		
		return 0;

	case WM_MOUSEMOVE:		
		CMouse::GetInstance()->SetXY(lParam);	
		if (mPthis->mMenu->IsSelectDraw())
		{
			//mPthis->mMenu->Drawing(mPthis->mBackbit, REVERSE);
		}
		x++;
		mPthis->mBackbit->ClearBitMap();
		return 0;

	case WM_TIMER:
	//{
		hdc = mPthis->mBackbit->GetBitmapDC();
		if (!test)
			Rectangle(hdc, x + 10, y + 10, x - 10, y - 10);
		mPthis->mBackbit->DeleteBitmapDC(hdc);
	//}
	InvalidateRect(hWnd, nullptr, false);
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		KillTimer(hWnd, 0);
		if (mPthis->mMenu != NULL)
		{
			delete mPthis->mMenu;
		}
		if (mPthis->mBackbit!= NULL)
		{
			delete mPthis->mBackbit;
		}
		return 0;

	case WM_PAINT:		
		hdc=BeginPaint(hWnd, &ps);	
		mPthis->mBackbit->SCreanDraw(hdc, 0,0);
		EndPaint(hWnd, &ps);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

void CWindowFrame::Run(MSG _msg)
{	
	TranslateMessage(&_msg);
	DispatchMessage(&_msg);	
}

void CWindowFrame::Initialize()
{
	this->BuildWindow();
}

void CWindowFrame::BuildWindow()
{	
	WNDCLASS WndClass;
	
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance = mHinstance;
	WndClass.lpfnWndProc=WndProc;
	WndClass.lpszClassName=TEXT("exam");
	WndClass.lpszMenuName=MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	mHWnd = CreateWindow(TEXT("exam"), TEXT("exam"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,(HMENU)NULL,mHinstance,NULL);

	ShowWindow(mHWnd, SW_SHOW);
}
