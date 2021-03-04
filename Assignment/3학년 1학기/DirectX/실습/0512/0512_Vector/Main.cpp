#include "MyMath.h"
#include "Vector3.h"
#include "MyFunction.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("Vector");

RECT w_rt;

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

	GetWindowRect(GetDesktopWindow(), &w_rt);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
	 w_rt.right / 4, w_rt.bottom / 5, 800, 800,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

Point2D StartPos, EndPos;

Point2D A_VectorPos[MAX_SIZE];
int A_Count = 0;
Point2D B_VectorPos[MAX_SIZE];
int B_Count = 0;
Point2D C_VectorPos[MAX_SIZE];
int C_Count = 0;

MyVector3 Vector3;

MyVector3* Vector3D[MAX_SIZE];
MyVector3 Vector3D_Temp[MAX_SIZE];

POINT MousePos;

float length;
TCHAR text[128];
char oper;

bool down = false;
int index = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	HDC DBDC;
	HBITMAP DB, oldDB;
	PAINTSTRUCT ps;
	static RECT rt;

	switch (iMessage) {

	case WM_CREATE:
		GetClientRect(hWnd, &rt);
		oper = '+';

		StartPos.x = -350;
		StartPos.y = -250;
		EndPos.x = 350;
		EndPos.y = 250;

		Vector3D[0] = new MyVector3(300, 90, 0);
		Vector3D[1] = new MyVector3(50, -50, 0);
		Vector3D[2] = new MyVector3();
		break;

	case WM_CHAR:
		switch (wParam) {
		case '+':
			oper = '+';
			break;

		case '-':
			oper = '-';
			break;

		case '*':
			oper = '*';
			break;
		}
		InvalidateRect(hWnd, &rt, false);
		break;

	case WM_LBUTTONDOWN:
		hdc = GetDC(hWnd);
		SetViewportOrgEx(hdc, rt.right / 2, rt.bottom / 2, nullptr);
		GetCursorPos(&MousePos);
		ScreenToClient(hWnd, &MousePos);

		MousePos.x -= rt.right / 2;
		MousePos.y -= rt.bottom / 2;

		for (int i = 0; i < MAX_SIZE; i++) {
			if (Vector3D[i] == nullptr) break;
			
			if (CollisionEnterPoint(Vector3D[i]->GetX() + 25, Vector3D[i]->GetY() + 5, MousePos.x, MousePos.y, 30, 15)) {
				index = i;
				down = true;
				break;
			}
		}

		ReleaseDC(hWnd, hdc);
		break;

	case WM_LBUTTONUP:
		down = false;
		break;

	case WM_MOUSEMOVE:
		if (down) {
			hdc = GetDC(hWnd);
			SetViewportOrgEx(hdc, rt.right / 2, rt.bottom / 2, nullptr);
			GetCursorPos(&MousePos);
			ScreenToClient(hWnd, &MousePos);

			MousePos.x -= rt.right / 2;
			MousePos.y -= rt.bottom / 2;

			Vector3D[index]->SetX(MousePos.x);
			Vector3D[index]->SetY(MousePos.y);

			InvalidateRect(hWnd, &rt, false);
			ReleaseDC(hWnd, hdc);
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);		
		
		DBDC = CreateCompatibleDC(hdc);
		SetGraphicsMode(hdc, GM_ADVANCED);
		DB = CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
		oldDB = (HBITMAP)SelectObject(DBDC, DB);

		FillRect(DBDC, &rt, (HBRUSH)GetStockObject(WHITENESS));
		SetViewportOrgEx(DBDC, rt.right / 2, rt.bottom / 2, nullptr);

		{
			Field(DBDC, StartPos, EndPos);
			Axis(DBDC, StartPos, EndPos);

			switch (oper) {
			case '+':
				*Vector3D[2] = *Vector3D[0] + *Vector3D[1];
				break;

			case '-':
				*Vector3D[2] = *Vector3D[0] - *Vector3D[1];
				break;

			case '*':
				*Vector3D[2] = *Vector3D[0] * 2;
				break;
			}

			for (int i = 0; i < MAX_SIZE; i++) {
				if (Vector3D[i] == nullptr) break;
				sprintf_s(text, "Vector%d", i + 1);
				TextOut(DBDC, Vector3D[i]->GetX(), Vector3D[i]->GetY(), text, strlen(text));
			}

			A_Count = SimpleLine(A_VectorPos, Vector3D[0]->GetX(), Vector3D[0]->GetY());
			for (int i = 0; i < A_Count; i++) {
				SetPixel(DBDC, A_VectorPos[i].x, A_VectorPos[i].y, RGB(255, 0, 0));
			}


			B_Count = SimpleLine(B_VectorPos, Vector3D[1]->GetX(), Vector3D[1]->GetY());
			for (int i = 0; i < B_Count; i++) {
				SetPixel(DBDC, B_VectorPos[i].x, B_VectorPos[i].y, RGB(0, 0, 255));
			}

			C_Count = SimpleLine(C_VectorPos, Vector3D[2]->GetX(), Vector3D[2]->GetY());
			for (int i = 0; i < C_Count; i++) {
				SetPixel(DBDC, C_VectorPos[i].x, C_VectorPos[i].y, RGB(255, 0, 255));
			}

			sprintf_s(text, "Vector1 Position - x : %0.2f / y : %0.2f", Vector3D[0]->GetX(), Viewport_Convert_Y(Vector3D[0]->GetY()));
			TextOut(DBDC, 0, 360 * -1, text, strlen(text));

			sprintf_s(text, "Vector2 Position - x : %0.2f / y : %0.2f", Vector3D[1]->GetX(), Viewport_Convert_Y(Vector3D[1]->GetY()));
			TextOut(DBDC, 0, 340 * -1, text, strlen(text));

			sprintf_s(text, "Vector1 와 Vector2 의 내적 : %0.2f", (Vector3.Dot(Vector3D[0]->Normalize(), Vector3D[1]->Normalize())));
			TextOut(DBDC, 0, 320 * -1, text, strlen(text));

			sprintf_s(text, "Vector1 와 Vector2 의 각도 : %0.2f", Vector3.GetAngle(Vector3D[0]->Normalize(), Vector3D[1]->Normalize()));
			TextOut(DBDC, 0, 300 * -1, text, strlen(text));

			if (oper == '*')
				sprintf_s(text, "Vector3 = Vector1 %c 2", oper);
			else
				sprintf_s(text, "Vector3 = Vector1 %c Vector2", oper);
			TextOut(DBDC, 0, 280 * -1, text, strlen(text));
		}

		SetViewportOrgEx(DBDC, 0, 0, nullptr);

		BitBlt(hdc, 0, 0, rt.right, rt.bottom, DBDC, 0, 0, SRCCOPY);
		SelectObject(DBDC, oldDB);

		DeleteObject(DB);
		DeleteDC(DBDC);

		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		for (int i = 0; i < MAX_SIZE; i++) {
			if (Vector3D[i] != nullptr) delete Vector3D[i];
		}
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
