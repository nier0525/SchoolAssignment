#include "MyMath.h"
#include <stdio.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("2015031090_Ω…¿Á√∂");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

Point2D StartPos;
Point2D EndPos;

POINT MousePos;

float ox, oy;
TCHAR text[128];

ListNode<Point2D*>* Point2DList = new ListNode<Point2D*>();
Node<Point2D*>* Search = nullptr;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rt;

	switch (iMessage) {

	case WM_CREATE:
		StartPos.x = -350;
		StartPos.y = -250;
		EndPos.x = 350;
		EndPos.y = 250;		
		break;

	case WM_LBUTTONDOWN:
		GetClientRect(hWnd, &rt);
		hdc = GetDC(hWnd);
		SetViewportOrgEx(hdc, rt.right / 2, rt.bottom / 2, nullptr);

		GetCursorPos(&MousePos);
		ScreenToClient(hWnd, &MousePos);

		Viewport_Convert_XY(StartPos, EndPos, (MousePos.x - (rt.right / 2)), (MousePos.y - (rt.bottom / 2)), &ox, &oy);
		sprintf_s(text, "%.1f , %.1f", ox, oy);
		Point2DList->Insert(new Point2D((MousePos.x - (rt.right / 2)), (MousePos.y - (rt.bottom / 2)), StartPos, EndPos, ox, oy));

		InvalidateRect(hWnd, nullptr, true);
		ReleaseDC(hWnd, hdc);
		break;

	case WM_RBUTTONDOWN:
		if (Point2DList->GetHead()->GetNext())
			Point2DList->Remove(Point2DList->GetHead()->GetNext()->GetData());
		InvalidateRect(hWnd, nullptr, true);
		break;

	case WM_PAINT:
		GetClientRect(hWnd, &rt);
		hdc = BeginPaint(hWnd, &ps);		
		SetViewportOrgEx(hdc, rt.right / 2, rt.bottom / 2, nullptr);

		Search = Point2DList->GetHead()->GetNext();

		if (Search)
			TextOut(hdc, (MousePos.x - (rt.right / 2)) - 30, (MousePos.y - (rt.bottom / 2)) - 30, text, strlen(text));

		while (Search) {
			Ellipse(hdc, Search->GetData()->x + 10, Search->GetData()->y + 10, Search->GetData()->x - 10, Search->GetData()->y - 10);
			Search = Search->GetNext();
		}

		Field(hdc, StartPos, EndPos);
		Axis(hdc, StartPos, EndPos);

		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
