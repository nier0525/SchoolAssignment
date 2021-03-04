#include "MyMath.h"
#include "Matrix4x4.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("심재철_2015031090");

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


// 전역 변수
Vector3 RectStartVertex[4];
Vector3 RectVertex[4][MAX_SIZE];
int Rect_Count[4];

Vector3 CircleVertex[MAX_SIZE * 2];
int Circle_Count;

POINT MousePos;
Vector3 DistancePos;

Vector3 Position;
Vector3 Scale;

Vector3 Distance;

Matrix4x4 Matrix;

bool flag = false;

// 사각형, 원 버텍스 정보 초기화
void Init()
{
	Position = Vector3();
	Scale = Vector3(100, 100, 0);

	RectStartVertex[0] = Vector3(Position.x - Scale.x, Viewport_Convert_Y(Position.y - Scale.y), 0);
	RectStartVertex[1] = Vector3(Position.x + Scale.x, Viewport_Convert_Y(Position.y - Scale.y), 0);
	RectStartVertex[2] = Vector3(Position.x + Scale.x, Viewport_Convert_Y(Position.y + Scale.y), 0);
	RectStartVertex[3] = Vector3(Position.x - Scale.x, Viewport_Convert_Y(Position.y + Scale.y), 0);

	Rect_Count[0] = SimpleLine(RectVertex[0], RectStartVertex[0].x, RectStartVertex[0].y, RectStartVertex[1].x, RectStartVertex[1].y);
	Rect_Count[1] = SimpleLine(RectVertex[1], RectStartVertex[1].x, RectStartVertex[1].y, RectStartVertex[2].x, RectStartVertex[2].y);
	Rect_Count[2] = SimpleLine(RectVertex[2], RectStartVertex[2].x, RectStartVertex[2].y, RectStartVertex[3].x, RectStartVertex[3].y);
	Rect_Count[3] = SimpleLine(RectVertex[3], RectStartVertex[3].x, RectStartVertex[3].y, RectStartVertex[0].x, RectStartVertex[0].y);

	Circle_Count = SimpleCircle(CircleVertex, Position.x, Position.y, 100);

	Distance = Vector3();
	DistancePos = Vector3();
}

// 사각형, 원 버텍스 정보 갱신
void Update_RectVectex()
{
	Vector3 result = Vector3();

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < Rect_Count[i]; j++)
		{
			result = RectVertex[i][j] * Matrix;
			RectVertex[i][j] = result;
		}
	}
}

void Update_CircleVertex()
{
	Vector3 result = Vector3();

	for (int i = 0; i < Circle_Count; i++)
	{
		result = CircleVertex[i] * Matrix;
		CircleVertex[i] = result;
	}
}

// 렌더링
void Draw(HDC hdc)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < Rect_Count[i]; j++) {
			SetPixel(hdc, RectVertex[i][j].x, RectVertex[i][j].y, BLACKNESS);
		}
	}

	for (int i = 0; i < Circle_Count; i++)
		SetPixel(hdc, CircleVertex[i].x, CircleVertex[i].y, BLACKNESS);
}

// 이동 트랜스 폼 함수
void Translate(Vector3 vector)
{
	Vector3 dis = vector - Position;

	Matrix4x4 T = Matrix4x4();
	T.Matrix4x4_Identity();
	T.Matrix4x4_SetupMatrix(MATRIX_TRANSLATE, dis);

	Position = vector;
	Matrix = T;
}

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
		Init();
		break;

	case WM_LBUTTONDOWN:
		hdc = GetDC(hWnd);
		SetViewportOrgEx(hdc, rt.right / 2, rt.bottom / 2, nullptr);
		GetCursorPos(&MousePos);
		ScreenToClient(hWnd, &MousePos);

		MousePos.x -= rt.right / 2;
		MousePos.y -= rt.bottom / 2;

		DistancePos.x = (MousePos.x - Position.x) / 20;
		DistancePos.y = (MousePos.y - Position.y) / 20;

		SetTimer(hWnd, 0, 10, nullptr);
		ReleaseDC(hWnd, hdc);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		DBDC = CreateCompatibleDC(hdc);
		SetGraphicsMode(hdc, GM_ADVANCED);
		DB = CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
		oldDB = (HBITMAP)SelectObject(DBDC, DB);

		FillRect(DBDC, &rt, (HBRUSH)GetStockObject(WHITENESS));
		SetViewportOrgEx(DBDC, rt.right / 2, rt.bottom / 2, nullptr);

		Draw(DBDC);

		SetViewportOrgEx(DBDC, 0, 0, nullptr);

		BitBlt(hdc, 0, 0, rt.right, rt.bottom, DBDC, 0, 0, SRCCOPY);
		SelectObject(DBDC, oldDB);

		DeleteObject(DB);
		DeleteDC(DBDC);

		EndPaint(hWnd, &ps);
		break;

	case WM_TIMER:
		Distance += DistancePos;

		if (DistancePos.x > 0 && Distance.x + DistancePos.x > MousePos.x) Distance.x -= Distance.x - MousePos.x;
		else if (DistancePos.x < 0 && Distance.x + DistancePos.x < MousePos.x) Distance.x -= Distance.x - MousePos.x;

		if (DistancePos.y > 0 && Distance.y + DistancePos.y > MousePos.y) {
			Distance.y -= Distance.y - MousePos.y;
			flag = true;
		}
		else if (DistancePos.y < 0 && Distance.y + DistancePos.y < MousePos.y) {
			Distance.y -= Distance.y - MousePos.y;
			flag = true;
		}

		Translate(Vector3(Distance.x, Distance.y, 0));
		Update_RectVectex();
		Update_CircleVertex();

		if (flag) {
			flag = false;
			KillTimer(hWnd, 0);
		}
		InvalidateRect(hWnd, nullptr, false);
		break;

	case WM_DESTROY:
		KillTimer(hWnd, 0);
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
