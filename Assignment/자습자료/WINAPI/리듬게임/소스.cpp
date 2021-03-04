#include "function.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); // CALLBACK 매크로
HINSTANCE g_hinst;   // 인스턴스 핸들
LPCTSTR lpszClass = TEXT("Game"); // 제목 표시물에 표시되는 내용


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
		300, 100, 800, 640, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	// 메세지 루프를 실행
	while (GetMessage(&Message, NULL, 0, 0)) {  // Queue 에 있는 메시지를
		TranslateMessage(&Message); // 키보드를 입력 메시지를 가공하여 프로그램을
									// 사용할 수 있도록
		DispatchMessage(&Message);  // 메세지를 윈도우의 메세지 처리 함수

	}

	return (int)Message.wParam; // 탈출 코드. 프로그램 종료
}

enum { EFF_D, EFF_F, EFF_J, EFF_K };
enum { PERFECT , GOOD , MISS };

#define D 0x44
#define F 0x46
#define J 0x4A
#define K 0x4B

#define UP 0
#define DOWN 1
#define MAX 30000
#define HITPOS 45

HWND hWndMain;
HDC hdc;

DWORD Eff, Note_t, Combo_t;
HANDLE hThread;

ULONG_PTR gdiplusToken;
GdiplusStartupInput gdiplusStartUpInput;

Image *eff[11];
Image *Gear, *Right, *Note;
Image *BT_D[2], *BT_F[2], *BT_J[2], *BT_K[2];
Image *Score_png[10];

HBITMAP Perfect_bit, Good_bit, Miss_bit, MaxCombo_bit;

HBRUSH brush, oldbrush;

int index[4];
int eff_x[4] , eff_y;
int light_x[4], light_y;
int life, life_x, life_y;
int note_x[4], note_y;
int score_x, score_y, score_w, score_h;
int judgment_h;

int NoteSpeed;
int Endline, Cntline;

int KeyState;
int score;
int combo, combo_stack[4];

int Perfect, Good, Miss;
int maxcombo;
int LifeStack;

int File_index;
int File_temp;


char NoteOrigin[MAX][4];
char NoteData[MAX][4];
char Stage[4][MAX];
char buf;

bool Clear;
bool once[4];
bool KeyDown[4];
bool Combo_show;
bool Judgment[3];

bool ThreadCheak;

TCHAR Score[MAX], Combo[MAX], MaxCombo[MAX];

// -----------------------------------------------------------------------------------------------------------------

void LoadNote() {
	// 파일 관련
	FILE* fp = fopen("Test.txt", "r");

	File_index = 0;

	if (fp == nullptr) {
		MessageBox(hWndMain, "노트 정보를 불러오지 못했습니다", "Error", MB_OK);
		ExitProcess(0);
	}

	while (!feof(fp)) {
		int i = 0;
		while (i < 4) {
			buf = fgetc(fp);

			if (buf == 'E') {
				break;
			}

			if (buf == 'O' || buf == 'X') {
				NoteOrigin[File_index][i] = buf;
				i++;
			}
		}		
		if (buf == 'E') {
			Endline = File_index - 1;
			Cntline = Endline;
			break;
		}
		File_index++;
	}
	fclose(fp);
	memcpy(NoteData, NoteOrigin , sizeof(NoteOrigin));
}

// -----------------------------------------------------------------------------------------------------------------

void Init() {
	// 그래픽 리소스
	{

		// 이펙트
		eff[0] = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG2));
		eff[1] = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG1));
		eff[2] = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG3));
		eff[3] = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG4));
		eff[4] = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG5));
		eff[5] = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG6));
		eff[6] = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG7));
		eff[7] = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG8));
		eff[8] = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG9));
		eff[9] = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG10));
		eff[10] = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG11));

		// 기어 & 라이트 & 버튼
		Gear = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG12));
		Note = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG13));
		Right = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG14));

		BT_D[UP] = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG16));
		BT_D[DOWN] = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG15));

		BT_F[UP] = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG17));
		BT_F[DOWN] = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG18));

		BT_J[UP] = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG19));
		BT_J[DOWN] = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG20));

		BT_K[UP] = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG21));
		BT_K[DOWN] = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG22));

		// 스코어
		Score_png[0] = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG23));
		Score_png[1] = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG24));
		Score_png[2] = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG25));
		Score_png[3] = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG26));
		Score_png[4] = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG27));
		Score_png[5] = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG28));
		Score_png[6] = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG29));
		Score_png[7] = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG30));
		Score_png[8] = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG31));
		Score_png[9] = LoadPNG(g_hinst, MAKEINTRESOURCE(IDB_PNG32));

		// 판정
		Perfect_bit = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP11));
		Good_bit = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP12));
		Miss_bit = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP13));
		MaxCombo_bit = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP14));
	}

	for (int i = 0; i < 4; i++) {
		index[i] = 10;
		combo_stack[i] = 0;
	}

	// 좌표
	for (int i = 0; i < 4; i++) {
		eff_x[i] = 4 + (i * 50);
		light_x[i] = 10 + (i * 50);
		note_x[i] = 10 + (i * 50);
	}
	eff_y = 428;
	light_y = 204;
	note_y = 0;

	life = 200;
	life_x = 224;
	life_y = 602;

	score_x = 87;
	score_y = 150;
	score_w = 44;
	score_h = 59;

	judgment_h = 35;

	// 기타
	KeyState = UP;
	score = 0;

	NoteSpeed = 1;
	Endline = 0;
	Cntline = 0;

	combo = 0;
	maxcombo = 0;
	Perfect = 0;
	Good = 0;
	Miss = 0;

	LifeStack = 0;

	Clear = false;
	Combo_show = false;
	ThreadCheak = false;

	for (int i = 0; i < 4; i++) {
		KeyDown[i] = false;
		once[i] = false;
	}

	for (int i = 0; i < 3; i++) {
		Judgment[i] = false;
	}

	LoadNote();
}

// -----------------------------------------------------------------------------------------------------------------

void DrawNote(HDC hdc) {

	// 노드
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 45; y++) {
			if (Stage[x][y] == 'O')
			{
				DrawPNG(hdc, *Note, note_x[x], (double)y * 10.3, 48, 16);
			}
		}
	}

	// 노트 이펙트
	for (int i = 0; i < 4; i++) {
		DrawPNG(hdc, *eff[index[i]], eff_x[i], eff_y, 60, 60);
	}
}

void DrawCombo(HDC hdc) {
	
	if (Combo_show) {

		if (combo_stack[0] > 9) {
			combo_stack[0] = 0;
			combo_stack[1]++;
		}

		if (combo_stack[1] > 9) {
			combo_stack[0] = 0;
			combo_stack[1] = 0;
			combo_stack[2]++;
		}

		if (combo_stack[2] > 9) {
			combo_stack[0] = 0;
			combo_stack[1] = 0;
			combo_stack[2] = 0;
			combo_stack[3]++;
		}

		if (combo_stack[0] < 10 && combo_stack[1] == 0 && combo_stack[2] == 0 && combo_stack[3] == 0) {
			DrawPNG(hdc, *Score_png[combo_stack[0]], score_x, score_y, score_w, score_h);
		}

		if (combo_stack[0] < 10 && combo_stack[1] != 0 && combo_stack[1] < 10 && combo_stack[2] == 0 && combo_stack[3] == 0) {
			DrawPNG(hdc, *Score_png[combo_stack[1]], score_x - 25, score_y, score_w, score_h);
			DrawPNG(hdc, *Score_png[combo_stack[0]], score_x + 25, score_y, score_w, score_h);
		}

		if (combo_stack[0] < 10 &&  combo_stack[1] < 10 && combo_stack[2] != 0 && combo_stack[2] < 10 && combo_stack[3] == 0) {
			DrawPNG(hdc, *Score_png[combo_stack[2]], score_x - 50, score_y, score_w, score_h);
			DrawPNG(hdc, *Score_png[combo_stack[1]], score_x, score_y, score_w, score_h);
			DrawPNG(hdc, *Score_png[combo_stack[0]], score_x + 50, score_y, score_w, score_h);
		}

		if (combo_stack[0] < 10 && combo_stack[1] < 10 && combo_stack[2] < 10 && combo_stack[3] != 0 && combo_stack[4] < 10) {
			DrawPNG(hdc, *Score_png[combo_stack[3]], score_x - 75, score_y, score_w, score_h);
			DrawPNG(hdc, *Score_png[combo_stack[2]], score_x - 25, score_y, score_w, score_h);
			DrawPNG(hdc, *Score_png[combo_stack[1]], score_x + 25, score_y, score_w, score_h);
			DrawPNG(hdc, *Score_png[combo_stack[0]], score_x + 75, score_y, score_w, score_h);
		}
	}
}

void DrawJudgment(HDC hdc) {
	if (Combo_show) {
		if (Judgment[PERFECT]) {
			DrawBitmap(hdc, 40, 75, 148, judgment_h, Perfect_bit, 0, 0, 0);
		}
		if (Judgment[GOOD]) {
			DrawBitmap(hdc, 40, 75, 148, judgment_h, Good_bit, 0, 0, 0);
		}
		if (Judgment[MISS]) {
			DrawBitmap(hdc, 40, 75, 148, judgment_h, Miss_bit, 0, 0, 0);
		}
	}
}

void DrawUI(HDC hdc) {
	// 기어 스킨
	DrawPNG(hdc, *Gear, 0, 0, 255, 602);

	DrawPNG(hdc, *BT_D[UP], 0, 0, 255, 602);
	DrawPNG(hdc, *BT_F[UP], 0, 0, 255, 602);
	DrawPNG(hdc, *BT_J[UP], 0, 0, 255, 602);
	DrawPNG(hdc, *BT_K[UP], 0, 0, 255, 602);

	// 노트 라이트 && 버튼
	if (KeyState == DOWN) {
		if (GetAsyncKeyState(D) && 0x8000) {
			DrawPNG(hdc, *BT_D[DOWN], 0, 0, 255, 602);
			DrawPNG(hdc, *Right, light_x[0], light_y, 48, 250);
		}
		if (GetAsyncKeyState(F) && 0x8000) {
			DrawPNG(hdc, *BT_F[DOWN], 0, 0, 255, 602);
			DrawPNG(hdc, *Right, light_x[1], light_y, 48, 250);
		}
		if (GetAsyncKeyState(J) && 0x8000) {
			DrawPNG(hdc, *BT_J[DOWN], 0, 0, 255, 602);
			DrawPNG(hdc, *Right, light_x[2], light_y, 48, 250);
		}
		if (GetAsyncKeyState(K) && 0x8000) {
			DrawPNG(hdc, *BT_K[DOWN], 0, 0, 255, 602);
			DrawPNG(hdc, *Right, light_x[3], light_y, 48, 250);
		}
	}

	// 노트
	DrawNote(hdc);

	// 콤보
	DrawCombo(hdc);

	// 판정
	DrawJudgment(hdc);

	// 기타 UI
	brush = CreateSolidBrush(RGB(0, 255, 0));
	oldbrush = (HBRUSH)SelectObject(hdc, brush);
	Rectangle(hdc, life_x - 2, life_y - life * 2 + 33, life_x + 2, life_y);
	SelectObject(hdc, oldbrush);
	DeleteObject(brush);

	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 255, 255));
	SetTextAlign(hdc, TA_RIGHT);

	wsprintf(Score, "%d", score);
	TextOut(hdc, 170, 565, Score, strlen(Score));
}

// -----------------------------------------------------------------------------------------------------------------

DWORD WINAPI EffectThread(LPVOID temp) {

	int i = (int)temp;

	index[i] = 0;

	for (;;) {
		index[i]++;
		Sleep(50);
		if (index[i] == 10) {
			return 0;
		}
	}
}

DWORD WINAPI ComboThread(LPVOID temp) {
	for (;;) {
		score_h += 20;
		judgment_h += 10;
		Sleep(40);
		score_h -= 20;
		judgment_h -= 10;
		ThreadCheak = false;
		return 0;
	}
}

// -----------------------------------------------------------------------------------------------------------------

void PerfectNote() {
	Perfect++;
	score += 300 * (1+(combo/25));
	if (life < 200) {
		LifeStack += 2;
	}

	for (int i = 0; i < 3; i++) {
		Judgment[i] = false;
	}
	Judgment[PERFECT] = true;
}

void GoodNote() {
	Good++;
	score += 100 * (1 + (combo / 25));
	if (life < 200) {
		LifeStack++;
	}

	for (int i = 0; i < 3; i++) {
		Judgment[i] = false;
	}
	Judgment[GOOD] = true;
}

void MissNote() {
	Miss++;
	life -= 10;

	combo = 0;
	Combo_show = false;
	for (int i = 0; i < 4; i++) {
		combo_stack[i] = 0;
	}

	for (int i = 0; i < 3; i++) {
		Judgment[i] = false;
	}
	Judgment[MISS] = true;
}

void NoteTouch(int x) {
	for (int i = 0; i < 6; i++) {
		if (Stage[x][HITPOS + i] == 'O') {
			Stage[x][HITPOS + i] = 'X';
			NoteData[Cntline - 45 + HITPOS + (i + 1)][x] = 'X';
			CloseHandle(CreateThread(nullptr, 0, EffectThread, (LPINT)x, 0, &Eff));
		
			Combo_show = true;

			combo++;
			combo_stack[0]++;

			if (combo >= maxcombo) {
				maxcombo = combo;
			}

			if (!ThreadCheak) {
				CloseHandle(CreateThread(nullptr, 0, ComboThread, nullptr, 0, &Combo_t));
				ThreadCheak = true;
			}
			KillTimer(hWndMain, 1);
			SetTimer(hWndMain, 1, 2000, nullptr);

			switch (i) {
			case 0:
			case 1:
			case 2:
				PerfectNote();
				break;
			case 3:
			case 4:
			case 5:
			case 6:
				GoodNote();
				break;
			}
		}
	}

	for (int i = 1; i < 9; i++) {
		if (Stage[x][HITPOS - i] == 'O') {
			Stage[x][HITPOS - i] = 'X';
			NoteData[Cntline - 45 + HITPOS - (i - 1)][x] = 'X';
			CloseHandle(CreateThread(nullptr, 0, EffectThread, (LPINT)x, 0, &Eff));
		
			Combo_show = true;

			combo++;
			combo_stack[0]++;

			if (combo >= maxcombo) {
				maxcombo = combo;
			}

			if (!ThreadCheak) {
				CloseHandle(CreateThread(nullptr, 0, ComboThread, nullptr, 0, &Combo_t));
				ThreadCheak = true;
			}
			KillTimer(hWndMain, 1);
			SetTimer(hWndMain, 1, 2000, nullptr);

			switch (i) {
			case 1:
			case 2:
				PerfectNote();
				break;
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				GoodNote();
				break;
			}
		}
	}
}

void HitNote() {
	if (KeyDown[0]) {
		NoteTouch(EFF_D);
		KeyDown[0] = false;
	}

	if (KeyDown[1]) {
		NoteTouch(EFF_F);
		KeyDown[1] = false;
	}

	if (KeyDown[2]) {
		NoteTouch(EFF_J);
		KeyDown[2] = false;
	}

	if (KeyDown[3]) {
		NoteTouch(EFF_K);
		KeyDown[3] = false;
	}
}

// -----------------------------------------------------------------------------------------------------------------

DWORD WINAPI NoteThread(LPVOID temp) {
	while (1) {

		Sleep(30);

		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 60; y++) {
				Stage[x][y] = NoteData[Cntline - 45 + y][x];
			}
		}
		Cntline--;

		for (int i = 0; i < 4; i++) {
			if (Stage[i][48] == 'O') {
				MissNote();
			}
		}

		if (Cntline == 0) {
			Clear = true;
			break;
		}

		if (LifeStack >= 20) {
			life += 10;			
			LifeStack = 0;
		}

		if (life <= 0) {
			Clear = false;
			break;
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------------------------------------------

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC backDC, DBDC;
	HBITMAP DB, oldDB;

	PAINTSTRUCT ps;
	RECT rt;

	switch (iMessage) {

	case WM_SIZE:
		GetClientRect(hWnd, &rt);
		break;

	case WM_CREATE:
		hWndMain = hWnd;
		GdiplusStartup(&gdiplusToken, &gdiplusStartUpInput, nullptr);

		Init();
		CloseHandle(CreateThread(nullptr, 0, NoteThread, nullptr, 0, &Note_t));

		SetTimer(hWnd, 0, 10, nullptr);
		break;

	case WM_KEYDOWN:
		KeyState = DOWN;
		
		//switch (wParam) {
		//case D:
		//	if (!once[0]) {
		//		KeyDown[0] = true;
		//		once[0] = true;
		//	}
		//	break;

		//case F:
		//	if (!once[1]) {
		//		KeyDown[1] = true;
		//		once[1] = true;
		//	}
		//	break;

		//case J:
		//	if (!once[2]) {
		//		KeyDown[2] = true;
		//		once[2] = true;
		//	}
		//	break;

		//case K:
		//	if (!once[3]) {
		//		KeyDown[3] = true;
		//		once[3] = true;
		//	}
		//	break;
		//}
		//HitNote();
		break;

	case WM_KEYUP:
		KeyState = UP;
		switch (wParam) {
		case D:
			KeyDown[0] = false;
			once[0] = false;
			break;

		case F:
			KeyDown[1] = false;
			once[1] = false;
			break;

		case J:
			KeyDown[2] = false;
			once[2] = false;
			break;

		case K:
			KeyDown[3] = false;
			once[3] = false;
			break;
		}
		break;

	case WM_TIMER:
		switch (wParam) {
		case 0:
			if (KeyState == DOWN) {
				if (GetAsyncKeyState(D) & 0x8000) {
					if (!once[0]) {
						KeyDown[0] = true;
						once[0] = true;
					}
				}

				if (GetAsyncKeyState(F) & 0x8000) {
					if (!once[1]) {
						KeyDown[1] = true;
						once[1] = true;
					}
				}

				if (GetAsyncKeyState(J) & 0x8000) {
					if (!once[2]) {
						KeyDown[2] = true;
						once[2] = true;
					}
				}

				if (GetAsyncKeyState(K) & 0x8000) {
					if (!once[3]) {
						KeyDown[3] = true;
						once[3] = true;
					}
				}
				HitNote();
			}
			InvalidateRect(hWnd, nullptr, false);
			break;

		case 1:
			Combo_show = false;
			break;
		}
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
		// ----------
	
		DrawUI(hdc);

		// 더블 버퍼 끝
		backDC = hdc;
		hdc = DBDC;
		DBDC = backDC;
		BitBlt(hdc, 0, 0, rt.right, rt.bottom, DBDC, 0, 0, SRCCOPY);
		SelectObject(DBDC, oldDB);
		DeleteObject(DB);
		DeleteDC(DBDC);
		DeleteDC(backDC);
		// ----------
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:

		for (int i = 0; i < 11; i++) {
			delete eff[i];
		}
		
		for (int i = 0; i < 2; i++) {
			delete BT_D[i];
			delete BT_F[i];
			delete BT_J[i];
			delete BT_K[i];
		}

		for (int i = 0; i < 10; i++) {
			delete Score_png[i];
		}

		delete Gear;
		delete Note;
		delete Right;

		DeleteObject(Perfect_bit);
		DeleteObject(Good_bit);
		DeleteObject(Miss_bit);
		DeleteObject(MaxCombo_bit);

		GdiplusShutdown(gdiplusToken);
		PostQuitMessage(0);
		return 0;


	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}