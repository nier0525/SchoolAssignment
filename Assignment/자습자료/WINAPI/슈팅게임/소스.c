#include <windows.h> // 윈도우 헤더파일
#include <time.h>
#include <mmsystem.h>
#include "resource.h"
#include <stdlib.h>

#pragma comment(lib,"winmm.lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); // CALLBACK 매크로
HINSTANCE g_hinst;   // 인스턴스 핸들
LPCTSTR lpszClass = TEXT("심재철_2015031090"); // 제목 표시물에 표시되는 내용


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) // main 함수
{
	HWND hWnd; // 윈도우 핸들 선언
	MSG Message; // 메세지 구조체 변수 선언
	WNDCLASS WndClass; // Windows Class 구조체 변수 선언

	g_hinst = hInstance;
	// hInstance 값을 외부에서도 사용할 수 있도록 전역변수에 값을 저장

	WndClass.cbClsExtra = 0; // 예약 영역. 지금은 사용 X
	WndClass.cbWndExtra = 0; // 예약 영역
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // 윈도우의 배경 색상을 지정
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW); // 윈도우의 마우스 포인터 모양을 지정
	WndClass.hIcon = LoadIcon(NULL, IDI_HAND); // 윈도우의 커서 모양 지정
	WndClass.hInstance = hInstance; // 윈도우 클래스를 등록하는 프로그램
	WndClass.lpfnWndProc = WndProc; // 윈도우 메세지 처리 함수 지정
	WndClass.lpszClassName = lpszClass; // 윈도우 클래스의 이름 지정
	WndClass.lpszMenuName = NULL; // 이 프로그램의 메뉴 지정
	WndClass.style = CS_HREDRAW | CS_DBLCLKS | CS_VREDRAW; // 윈도우의 스타일을 정의

	RegisterClass(&WndClass); // WNDCLASS 구조체의 번지를 전달

	// 윈도우를 생성하고 창에 표시하는 기능을 수행
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, /*CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT*/
		400, 100, 650, 950, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	// 메세지 루프를 실행
	while (GetMessage(&Message, NULL, 0, 0)) {  // Queue 에 있는 메시지를

		if ((Message.message == WM_KEYDOWN || Message.message == WM_KEYUP) && Message.hwnd != hWnd) {
			PostMessage(hWnd, Message.message, Message.wParam, Message.lParam);
		}
		TranslateMessage(&Message); // 키보드를 입력 메시지를 가공하여 프로그램을
									// 사용할 수 있도록
		DispatchMessage(&Message);  // 메세지를 윈도우의 메세지 처리 함수

	}

	return (int)Message.wParam; // 탈출 코드. 프로그램 종료
}

#include <math.h>
#define Z 0x5A
#define X 0x58
#define MAX 15

static RECT rt;
static HBITMAP DB, oldDB;

static HBITMAP play, playL, skill, skillL, runp, runpL;
static HBITMAP play2, playL2, runp2, runpL2, runp3, runpL3;
static HBITMAP background, bulletp, impactp, uI, startlogo;
static HBITMAP mons, monL, mons2, monL2, mons3, monL3, mons4, monL4;

static BOOL left, run, attak, start, stop = FALSE;
static BOOL base = TRUE;
static BOOL right = TRUE;
static BOOL impact = FALSE;
static BOOL right2 = FALSE;
static BOOL left2 = TRUE;
static BOOL bitmapgaduri = TRUE;
static BOOL att, critical = FALSE;
static BOOL KeyDown = FALSE;
static BOOL KeyDown2 = FALSE;

static int move = 0;
static int move2 = 0;

static TCHAR str[128], str1[128], str2[128];

static int motion = 1;
static int runt = 0;
static int play_x, play_y;
static int im_x, im_y;

static int j, temp;

static int raddam_x[2] = { 0,600 };

static int life_x, life_x2;
static int life_y, life_y2;

static int damege = 20;
static int criup = 2;
static int bouns = 0;

static int cri[10] = { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 };
static int speed = 1;
static int count = 0;
static int count2 = 0;

void DrawBitmap(HDC hdc, int x, int y, int brx, int bry, HBITMAP hBitmap, int r, int g, int b) {
	HDC MemDC;
	HBITMAP OldBitMap;
	int bx, by;
	BITMAP bitmap;

	MemDC = CreateCompatibleDC(hdc);
	OldBitMap = (HBITMAP)SelectObject(MemDC, hBitmap);

	GetObject(hBitmap, sizeof(BITMAP), &bitmap);

	bx = bitmap.bmWidth;
	by = bitmap.bmHeight;

	GdiTransparentBlt(hdc, x, y, brx, bry, MemDC, 0, 0, bx, by, RGB(r, g, b));
	//BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, SRCCOPY);

	SelectObject(MemDC, OldBitMap);
	DeleteDC(MemDC);
}

struct Bullet {
	BOOL on;
	POINT pos;
};

struct Jump {
	BOOL on;
	double stack;
};

struct CharacterStatus {
	int MHP;
	int HP;
	struct Jump jump;
}my;

struct Monster {
	int x, y;
	int HP;
	int MHP;
	BOOL bone;
};

double Lengthpts(int x1, int y1, int x2, int y2) {
	return (sqrt((float)((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1)))); // 두 좌표간의 거리 계산
}

BOOL InCircle(int x, int y, int mx, int my) {
	if (Lengthpts(x, y, mx, my) < 32) return TRUE;
	else return FALSE;
}

BOOL InCircle2(int x, int y, int mx, int my) {
	if (Lengthpts(x, y, mx, my) < 36) return TRUE;
	else return FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	HDC backDC, DBDC;
	PAINTSTRUCT ps;
	static RECT crt = { 0,300,650,525 };
	static HBRUSH lifebar, oldlifebar;

	static struct Bullet bullet[MAX];
	static struct Bullet bullet2[MAX];
	static struct Monster mon;

	switch (iMessage) {

	case WM_SIZE:
		GetClientRect(hWnd, &rt); // 이동 시 창크기를 받음
		return 0;

	case WM_CREATE:
		GetClientRect(hWnd, &rt);
		srand((unsigned int)time(NULL));
		// 비트맵 불러오기
		if (bitmapgaduri) {

			startlogo = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP23));
			background = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP5));
			uI = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP33));
			startlogo = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP23));

			play = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP1));
			playL = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP9));
			play2 = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP2));
			playL2 = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP10));

			bulletp = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP18));

			skill = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP3));
			skillL = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP11));

			runp = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP6));
			runp2 = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP7));
			runp3 = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP8));

			runpL = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP13));
			runpL2 = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP14));
			runpL3 = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP15));

			mons = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP20));
			mons2 = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP21));
			mons3 = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP30));
			mons4 = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP31));

			monL = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP27));
			monL2 = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP28));
			monL3 = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP24));
			monL4 = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP25));

			//	impactp = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP19));

		}
		// -------------------------------

		// PlaySound(TEXT("bgm.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		
		my.MHP = 100;
		my.HP = 100;

		mon.MHP = 100;
		mon.HP = 100;

		play_x = 200;	play_y = 525;

		my.jump.on = FALSE;

		bullet[count].on = FALSE;
		bullet2[count].on = FALSE;

		mon.bone = FALSE;

		// SetStatus(stage);

		SetTimer(hWnd, 1, 100, NULL); // 프레임 구현
		SetTimer(hWnd, 2, 40, NULL); // 점프 구현
		SetTimer(hWnd, 3, 10, NULL); // 행동 구현
		return 0;

	case WM_CHAR:
		switch (wParam) {
		case 's':
			for (int j = 0; j < count; j++) {
				if (bullet[j].on)
					bullet[j].on = FALSE;
				if (bullet2[j].on)
					bullet2[j].on = FALSE;
			}

			count = 0;
			count2 = 0;

			stop = TRUE;
			start = FALSE;
			break;

		case 'S':
			for (int j = 0; j < count; j++) {
				if (bullet[j].on)
					bullet[j].on = FALSE;
				if (bullet2[j].on)
					bullet2[j].on = FALSE;
			}
			count = 0;
			count2 = 0;

			stop = TRUE;
			start = FALSE;
			break;
		}
		return 0;

	case WM_KEYDOWN:

		switch (wParam) {

		case VK_RETURN:
			start = TRUE;
			stop = FALSE;
			break;

		case VK_ESCAPE:
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
			KillTimer(hWnd, 3);
			ExitProcess(0);
			break;
		}
		return 0;

	case WM_KEYUP:
		switch (wParam) {
		case Z:
			KeyDown = FALSE;
			break;

		case X:
			KeyDown2 = FALSE;
			break;

		case VK_LEFT:
			run = FALSE;
			base = TRUE;
			break;

		case VK_RIGHT:
			run = FALSE;
			base = TRUE;
			break;
		}
		return 0;

	case WM_TIMER:
		GetClientRect(hWnd, &crt);
		if (!stop) {
			switch (wParam)
			{
			case 1:
				move += 1;  // 공격 행동 프레임

				if (move >= 10) {
					base = TRUE;
					attak = FALSE;
					move = 0.0;
				}

				move2 += 1; // 이동 프레임
				if (move2 >= 1) runt = 0;
				if (move2 >= 2) runt = 1;
				if (move2 >= 3) runt = 0;
				if (move2 >= 4) {
					runt = 2;
					move2 = 0;
				}
				break;

			case 2: // 점프 구현
				if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
					if (play_x <= 0) {
						play_x = 0;
						run = FALSE;
						base = TRUE;
					}
					else play_x -= 20;

					run = TRUE;
					base = FALSE;
					attak = FALSE;

					left = TRUE;
					right = FALSE;

				}
				if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
					if (play_x >= 520) {
						play_x = 520;
						run = FALSE;
						base = TRUE;
					}
					else play_x += 20;

					run = TRUE;
					base = FALSE;
					attak = FALSE;

					left = FALSE;
					right = TRUE;
				}

				if (!KeyDown2) {
					if (GetAsyncKeyState(X) & 0x8000) {
						my.jump.on = TRUE;
						KeyDown2 = TRUE;
					}
				}

				if (my.jump.on) {
					my.jump.stack += 10.0;
					play_y = 525 - 160 * sin((double)my.jump.stack * 3.14 / 180.0);
					if (my.jump.stack == 180.0) {
						my.jump.stack = 0;
						my.jump.on = FALSE;
					}
				}

				if (start) {
					if (!mon.bone)
					{
						mon.bone = TRUE;
						mon.y = 555;
						mon.x = raddam_x[rand() % 2];
						mon.HP = 100;

						j = rand() % 2;
					}
				}
				break;

			case 3: // 좌우 행동
				if (!KeyDown) {
					if (GetAsyncKeyState(Z) & 0x8000) {

						temp = cri[rand() % 10];

						if (right) {
							if (!bullet[count].on) {
								bullet[count].on = TRUE;
								bullet[count].pos.x = play_x + 130;
								bullet[count].pos.y = play_y + 60;
							}
							count++;
						}

						if (left) {
							if (!bullet2[count2].on) {
								bullet2[count2].on = TRUE;
								bullet2[count2].pos.x = play_x - 10;
								bullet2[count2].pos.y = play_y + 60;
							}
							count2++;
						}

						KeyDown = TRUE;

						motion = 1;

						attak = TRUE;
						base = FALSE;

						if (run == TRUE) attak = FALSE;
					}
				}
				break;
			}
		}
		InvalidateRect(hWnd, &crt, FALSE);
		break;


	case WM_PAINT:
		GetClientRect(hWnd, &rt);
		hdc = BeginPaint(hWnd, &ps);
		// 더블 버퍼
		DBDC = CreateCompatibleDC(hdc);
		DB = CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
		oldDB = (HBITMAP)SelectObject(DBDC, DB);
		PatBlt(DBDC, 0, 0, rt.right, rt.bottom, WHITENESS);
		backDC = hdc;
		hdc = DBDC;
		DBDC = backDC;
		// ---------
		DrawBitmap(hdc, 0, 0, rt.right, rt.bottom, background, 5, 0, 0);
		// 시작&중지 화면
		if (!start) {
			if (move >= 0.6 || stop == TRUE)
				DrawBitmap(hdc, 120, 300, 400, 50, startlogo, 255, 255, 255);
			else
				DrawBitmap(hdc, 120, 300, 0, 0, startlogo, 255, 255, 255);
		}
		// ------------------

		// UI 
		DrawBitmap(hdc, 450, 850, 200, 50, uI, 255, 0, 0);
		DrawBitmap(hdc, 450, 780, 200, 50, uI, 255, 0, 0);
		// ------------------------------------------

		// 기본 자세
		if (base) {
			if (move >= 6) {
				if (right) {
					DrawBitmap(hdc, play_x, play_y, 150, 150, play, 255, 0, 0);
				}
				if (left) {
					DrawBitmap(hdc, play_x, play_y, 150, 150, playL, 255, 0, 0);
				}
			}
			else
			{
				if (right) {
					DrawBitmap(hdc, play_x, play_y, 150, 150, play2, 255, 0, 0);
				}
				if (left) {
					DrawBitmap(hdc, play_x, play_y, 150, 150, playL2, 255, 0, 0);
				}
			}
		}
		// ---------

		// 공격
		if (attak) {
			if (motion == 1) {
				if (right) {
					DrawBitmap(hdc, play_x, play_y, 150, 150, skill, 255, 0, 0);
				}
				if (left) {
					DrawBitmap(hdc, play_x, play_y, 150, 150, skillL, 255, 0, 0);
				}
			}
		}
		// ---------

		// 이동
		if (run) {
			if (runt == 0) {
				if (right) {
					DrawBitmap(hdc, play_x, play_y, 150, 150, runp, 255, 0, 0);
				}
				if (left) {
					DrawBitmap(hdc, play_x, play_y, 150, 150, runpL, 255, 0, 0);
				}
			}
			if (runt == 1) {
				if (right) {
					DrawBitmap(hdc, play_x, play_y, 150, 150, runp2, 255, 0, 0);
				}
				if (left) {
					DrawBitmap(hdc, play_x, play_y, 150, 150, runpL2, 255, 0, 0);
				}
			}
			if (runt == 2) {
				if (right) {
					DrawBitmap(hdc, play_x, play_y, 150, 150, runp3, 255, 0, 0);
				}
				if (left) {
					DrawBitmap(hdc, play_x, play_y, 150, 150, runpL3, 255, 0, 0);
				}
			}
		}
		// ---------

		// 총탄
		for (int j = 0; j < count; j++) {

			if (bullet[j].on) {
				bullet[j].pos.x += 5;

				if (bullet[j].pos.x >= rt.right - 36) {
					bullet[j].on = FALSE;

					for (int i = j; i < count - 1; i++) {
						bullet[i] = bullet[i + 1];
					}
					bullet[count - 1].on = FALSE;
					count--;
				}

				if (InCircle(mon.x, mon.y, bullet[j].pos.x, bullet[j].pos.y))
				{
					bullet[j].on = FALSE;
					att = TRUE;

					for (int i = j; i < count - 1; i++) {
						bullet[i] = bullet[i + 1];
					}
					bullet[count - 1].on = FALSE;
					count--;
				}
				DrawBitmap(hdc, bullet[j].pos.x, bullet[j].pos.y, 32, 32, bulletp, 255, 255, 255);
			}
		}

		for (int j = 0; j < count2; j++) {
			if (bullet2[j].on) {
				bullet2[j].pos.x -= 5;

				if (bullet2[j].pos.x <= rt.left) {
					bullet2[j].on = FALSE;

					for (int i = j; i < count2 - 1; i++) {
						bullet2[i] = bullet2[i + 1];
					}
					bullet2[count2 - 1].on = FALSE;
					count2--;
				}

				if (InCircle(mon.x, mon.y, bullet2[j].pos.x, bullet2[j].pos.y))
				{
					bullet2[j].on = FALSE;
					att = TRUE;

					for (int i = j; i < count2 - 1; i++) {
						bullet2[i] = bullet2[i + 1];
					}
					bullet2[count2 - 1].on = FALSE;
					count2--;
				}
				DrawBitmap(hdc, bullet2[j].pos.x, bullet2[j].pos.y, 32, 32, bulletp, 255, 255, 255);
			}
		}

		if (att) {
			mon.HP -= damege;
			if (temp == 1) {
				mon.HP -= damege * criup;
			}
			if (mon.HP <= 0) {
				mon.bone = FALSE;
				mon.y = 0;
			}
			att = FALSE;
		}

		// ---------

		//이펙트
		/*
		if (impact)
		{
			DrawBitmap(hdc, im_x, im_y, 32, 32, impactp, 255, 0, 0);
		}
		*/
		// ------------

		// 몬스터
		if (mon.bone) {
			if (!stop) {
				if (mon.x > play_x) {
					mon.x -= speed;
					right2 = FALSE;
					left2 = TRUE;
				}
				if (mon.x < play_x) {
					mon.x += speed;
					right2 = TRUE;
					left2 = FALSE;
				}
			}
			if (right2) {
				if (j == 0) {
					if (move >= 6) {
						DrawBitmap(hdc, mon.x, mon.y, 120, 120, mons, 255, 0, 0);
					}
					else {
						DrawBitmap(hdc, mon.x, mon.y, 120, 120, mons2, 255, 0, 0);
					}
				}
				if (j == 1) {
					if (move >= 6) {
						DrawBitmap(hdc, mon.x, mon.y, 120, 120, mons3, 255, 0, 0);
					}
					else {
						DrawBitmap(hdc, mon.x, mon.y, 120, 120, mons4, 255, 0, 0);
					}
				}
			}

			//---------------------------------------------------------------------------------

			if (left2) {
				if (j == 0) {
					if (move >= 6) {
						DrawBitmap(hdc, mon.x, mon.y, 120, 120, monL, 255, 0, 0);
					}

					else {
						DrawBitmap(hdc, mon.x, mon.y, 120, 120, monL2, 255, 0, 0);
					}
				}

				if (j == 1) {
					if (move >= 6) {
						DrawBitmap(hdc, mon.x, mon.y, 120, 120, monL3, 255, 0, 0);
					}

					else {
						DrawBitmap(hdc, mon.x, mon.y, 120, 120, monL4, 255, 0, 0);
					}
				}

			}
			if (InCircle2(mon.x, mon.y, play_x, play_y))
			{
				mon.bone = FALSE;
				mon.y = 0;
				my.HP -= 20;
				if (my.HP <= 0) {
					my.HP = 0;
					KillTimer(hWnd, 1);
					KillTimer(hWnd, 2);
					KillTimer(hWnd, 3);
					MessageBox(hWnd, "게임 종료", "life Zero", MB_OK);
					ExitProcess(0);
				}
				if (my.HP >= 100) my.HP = 100;
			}
		}
		// -----------------------------------------

		// 체력
		// 풀피
		lifebar = CreateSolidBrush(RGB(255, 0, 0));
		oldlifebar = (HBRUSH)SelectObject(hdc, lifebar);

		Rectangle(hdc, play_x + 30, play_y - 20, (play_x + 20) + my.MHP, play_y - 15);
		Rectangle(hdc, mon.x + 20, mon.y - 20, (mon.x + 10) + mon.MHP, mon.y - 15);

		SelectObject(hdc, oldlifebar);
		DeleteObject(lifebar);

		// 현재피
		lifebar = CreateSolidBrush(RGB(85, 255, 102));
		oldlifebar = (HBRUSH)SelectObject(hdc, lifebar);

		Rectangle(hdc, play_x + 30, play_y - 20, (play_x + 20) + my.HP, play_y - 15);
		Rectangle(hdc, mon.x + 20, mon.y - 20, (mon.x + 10) + mon.HP, mon.y - 15);

		SelectObject(hdc, oldlifebar);
		DeleteObject(lifebar);
		// -----------------

		wsprintf(str, "%d", count);
		wsprintf(str1, "%d", count2);
		wsprintf(str2, "%d", move);

		TextOut(hdc, 150, 20, str, strlen(str));
		TextOut(hdc, 150, 40, str1, strlen(str1));
		TextOut(hdc, 150, 60, str2, strlen(str2));

		// -----------------

		// 더블 버퍼 마무리
		backDC = hdc;
		hdc = DBDC;
		DBDC = backDC;
		BitBlt(hdc, 0, 0, rt.right, rt.bottom, DBDC, 0, 0, SRCCOPY);
		SelectObject(DBDC, oldDB);
		DeleteObject(DB);
		DeleteDC(DBDC);
		DeleteDC(backDC);
		DeleteDC(hdc);
		// -----------------
		
		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:

		DeleteObject(background);
		DeleteObject(uI);
		DeleteObject(startlogo);
		DeleteObject(play);
		DeleteObject(playL);
		DeleteObject(play2);
		DeleteObject(playL2);
		DeleteObject(runpL);
		DeleteObject(runpL2);
		DeleteObject(runpL3);
		DeleteObject(runp);
		DeleteObject(runp2);
		DeleteObject(runp3);
		DeleteObject(mons);
		DeleteObject(monL);
		DeleteObject(mons2);
		DeleteObject(monL2);
		DeleteObject(mons3);
		DeleteObject(monL3);
		DeleteObject(mons4);
		DeleteObject(monL4);
		DeleteObject(bulletp);
		DeleteObject(skill);
		DeleteObject(skillL);

		KillTimer(hWnd, 1);
		KillTimer(hWnd, 2);
		KillTimer(hWnd, 3);

		// DeleteObject(impactp);
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
};