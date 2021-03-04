#include "SocketFunction.h"
#include <windows.h>
#include "resource.h"

#define MAXSTAGE 3
#define BW 16
#define BH 16

// 게임에 필요한 변수
// n = 자신 , p = 상대
char ns[18][21];
char ps[18][21];

int nStage, pState;
int nx, ny;
int px, py;

// 비트맵 리소스
HBITMAP hBit[5];

bool flag = false;

void Render(HDC hdc);
void DrawBitmap(HDC hdc,int x,int y, int _bx, int _by,HBITMAP hBit);

// 클라이언트 정보
ClientInfo ptr;

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("Sokoban");

DWORD WINAPI ClientMain(LPVOID arg);
DWORD WINAPI RecvProcess(LPVOID arg);

// 윈도우 초기화
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	  ,LPSTR lpszCmdParam,int nCmdShow)
{
	// 윈도우 값 세팅
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;
	
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=NULL;
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	// 윈도우 생성
	hWnd=CreateWindow(lpszClass,lpszClass, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT,CW_USEDEFAULT,0,0,NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	
	// 클라이언트 스레드 생성
	CreateThread(nullptr, 0, ClientMain, nullptr, 0, nullptr);

	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

// 클라이언트 스레드
DWORD WINAPI ClientMain(LPVOID arg)
{
	int retval;

	// ClientInfo Setting
	{
		WSADATA wsa;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return -1;

		ZeroMemory(&ptr, sizeof(ClientInfo));
		ptr.sock = socket(AF_INET, SOCK_STREAM, 0);
		if (ptr.sock == INVALID_SOCKET) err_quit("socket()");

		SOCKADDR_IN addr;
		ZeroMemory(&addr, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(PORT);
		addr.sin_addr.s_addr = inet_addr(IP);
		memcpy(&ptr.addr, &addr, sizeof(addr));

		retval = connect(ptr.sock, (SOCKADDR*)&ptr.addr, sizeof(SOCKADDR));
		if (retval == SOCKET_ERROR) err_quit("connect()");

		ptr.state = WAIT_STATE;

		ptr.hWrite = CreateEvent(nullptr, false, false, nullptr);

		// 수신 스레드 생성
		CreateThread(nullptr, 0, RecvProcess, nullptr, 0, 0);
	}

	bool Endflag = false;

	// 처음데 접속햇다고 송신한다.
	PackPacket(ptr.s_buf, JOIN, ptr.size);
	retval = send(ptr.sock, ptr.s_buf, ptr.size, 0);
	if (retval == SOCKET_ERROR) err_quit("send");

	while (!Endflag)
	{
		// 읽기 이벤트가 켜질 때까지 대기
		WaitForSingleObject(ptr.hWrite, INFINITE);
		ZeroMemory(ptr.s_buf, sizeof(ptr.s_buf));

		// 눌린 키값에 따라서 작업 분류
		switch (ptr.key)
		{
		// 게임 종료
		case Q:
			// 종료 전 정상 종료를 알리기 위해 빈 버퍼를 송신해준다.
			ZeroMemory(ptr.s_buf, sizeof(ptr.s_buf));
			retval = send(ptr.sock, ptr.s_buf, BUFSIZE, 0);
			if (retval == SOCKET_ERROR) err_quit("send");

			// 반복문 탈출
			Endflag = true;
			break;

		// 게임 리셋 or 다시 매칭
		case R:
			// 게임 리셋 요청
			if (ptr.state == GAME_STATE)
			{
				// 게임 리셋을 요청하는 패킷을 송신한다.
				PackPacket(ptr.s_buf, RESET, ptr.size);
				retval = send(ptr.sock, ptr.s_buf, ptr.size, 0);
				if (retval == SOCKET_ERROR) err_quit("send");
			}
			// 다시 매칭하기를 요청
			if (ptr.state == WIN_STATE || ptr.state == LOSE_STATE)
			{
				// 게임이 끝난 후, 다시 매칭하기를 요청하는 패킷을 송신한다.
				PackPacket(ptr.s_buf, JOIN, ptr.size);
				retval = send(ptr.sock, ptr.s_buf, ptr.size, 0);
				if (retval == SOCKET_ERROR) err_quit("send");
				
				ptr.state = WAIT_STATE;
				InvalidateRect(hWndMain, nullptr, false);
			}
			break;

		// 방향키에 따라 이동값을 수신해준다.
		case LEFT:
			PackPacket(ptr.s_buf, MOVE, -1, 0, ptr.size);
			retval = send(ptr.sock, ptr.s_buf, ptr.size, 0);
			if (retval == SOCKET_ERROR) err_quit("send");
			break;

		case RIGHT:
			PackPacket(ptr.s_buf, MOVE, 1, 0, ptr.size);
			retval = send(ptr.sock, ptr.s_buf, ptr.size, 0);
			if (retval == SOCKET_ERROR) err_quit("send");
			break;

		case UP:
			PackPacket(ptr.s_buf, MOVE, 0, -1, ptr.size);
			retval = send(ptr.sock, ptr.s_buf, ptr.size, 0);
			if (retval == SOCKET_ERROR) err_quit("send");
			break;

		case DOWN:
			PackPacket(ptr.s_buf, MOVE, 0, 1, ptr.size);
			retval = send(ptr.sock, ptr.s_buf, ptr.size, 0);
			if (retval == SOCKET_ERROR) err_quit("send");
			break;
		}
	}

	// 마무리 정리 작업
	for (int i = 0; i < 5; i++) {
		DeleteObject(hBit[i]);
	}

	closesocket(ptr.sock);
	WSACleanup();

	return 0;
}

// 수신 스레드
DWORD WINAPI RecvProcess(LPVOID arg)
{
	int retval;

	while (1)
	{
		// 가변 길이로 온 수신 패킷을 분할 작업
		if (!PacketRecv(ptr.sock, ptr.r_buf)) break;

		// 프로토콜에 따라 작업 분류
		switch (GetProtocol(ptr.r_buf))
		{
		// 게임 상대를 찾았다는 패킷을 받았을 때, 게임 정보 세팅을 요청한다.
		case MATCH:
			PackPacket(ptr.s_buf, SETTING, ptr.size);
			retval = send(ptr.sock, ptr.s_buf, ptr.size, 0);
			if (retval == SOCKET_ERROR) err_quit("send");
			break;

		// 세팅된 게임 정보를 각각 변수에 저장한다.
		// 그리고 게임 상태로 변경해준다.
		case MAPSETTING:
			UnPackPacket(ptr.r_buf, ns, ps, nStage, pState, nx, ny, px, py);
			px = nx;
			py = ny;

			ptr.state = GAME_STATE;
			InvalidateRect(hWndMain, nullptr, false);
			break;

		// 게임 진행 중 변동 사항 때마다 렌더링을 업데이트 해준다.
		// 상대가 움직이거나, 자신이 움직일 때 모두 업데이트 패킷을 받는다.
		case UPDATE:
			UnPackPacket(ptr.r_buf, ns, ps, nStage, pState, nx, ny, px, py);
			InvalidateRect(hWndMain, nullptr, false);
			break;

		// 승리했을 경우 승리 상태로 변경
		case WIN: 
			ptr.state = WIN_STATE;
			InvalidateRect(hWndMain, nullptr, false);
			break;

		// 패배했을 경우 패배 상태로 변경
		case LOSE:
			ptr.state = LOSE_STATE;
			InvalidateRect(hWndMain, nullptr, false);
			break;

		// 다시 다른 상대와 매칭 되기를 원하면 리매치 패킷을 송신한다.
		// 그 후, 다시 대기 상태로 변경한다.
		case REMATCH:
			PackPacket(ptr.s_buf, JOIN, ptr.size);
			retval = send(ptr.sock, ptr.s_buf, ptr.size, 0);
			if (retval == SOCKET_ERROR) err_quit("send");

			ptr.state = WAIT_STATE;
			InvalidateRect(hWndMain, nullptr, false);
			break;
		}
	}
	return 0;
}

// 윈도우 작업 스레드
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	// 각각 변수들
	HDC hdc, BackDC;
	HBITMAP Back, OldBack;

	PAINTSTRUCT ps;
	RECT crt;
	TCHAR Message[256];
				
	switch (iMessage) {
	case WM_CREATE:
		// 윈도우 창을 임의의 값으로 변경한다.
		hWndMain = hWnd;
		SetRect(&crt, 0, 0, 1120, BH * 18);
		AdjustWindowRect(&crt, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);
		SetWindowPos(hWnd, NULL, 0, 0, crt.right - crt.left, crt.bottom - crt.top,
			SWP_NOMOVE | SWP_NOZORDER);

		// 비트맵 소스를 불러온다.
		for (int i = 0; i < 5; i++) {
			hBit[i] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_WALL + i));
		}
		return 0;

	case WM_PAINT:
		// 더블 버퍼링 작업
		GetClientRect(hWnd, &crt);
		hdc=BeginPaint(hWnd, &ps);

		BackDC = CreateCompatibleDC(hdc);
		SetGraphicsMode(hdc, GM_ADVANCED);
		Back = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
		OldBack = (HBITMAP)SelectObject(BackDC, Back);

		FillRect(BackDC, &crt, (HBRUSH)GetStockObject(WHITENESS));

		// 게임 렌더링
		Render(BackDC);

		BitBlt(hdc, 0, 0, crt.right, crt.bottom, BackDC, 0, 0, SRCCOPY);
		SelectObject(BackDC, OldBack);

		DeleteObject(Back);
		DeleteDC(BackDC);

		EndPaint(hWnd, &ps);
		return 0;

	// 키 입력에 따라 정해진 키값을 입력 후, 쓰기 이벤트를 깨워준다.
	// 즉, 입력할 때마다 서버에게 키값 정보를 송신해준다.
	case WM_KEYDOWN:
		if (ptr.state == GAME_STATE)
		{
			switch (wParam) {
			case VK_LEFT:
				ptr.key = LEFT;
				SetEvent(ptr.hWrite);
				break;

			case VK_RIGHT:
				ptr.key = RIGHT;
				SetEvent(ptr.hWrite);
				break;

			case VK_UP:
				ptr.key = UP;
				SetEvent(ptr.hWrite);
				break;

			case VK_DOWN:
				ptr.key = DOWN;
				SetEvent(ptr.hWrite);
				break;
			}
		}

		switch (wParam)
		{
		case 'R':
			ptr.key = R;
			SetEvent(ptr.hWrite);
			break;

		case 'Q':
			ptr.key = Q;
			SetEvent(ptr.hWrite);

			PostQuitMessage(0);
			break;
		}
		return 0;

	// 윈도우 종료 시 정리 작업을 위해 쓰기 이벤트를 켜준다.
	case WM_DESTROY:
		ptr.key = Q;
		SetEvent(ptr.hWrite);

		PostQuitMessage(0);
		return 0;
	}

	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

// 렌더링
void Render(HDC hdc)
{
	int x, y;
	int iBit;
	TCHAR Message[256];

	// 게임 상태일 경우 출력
	if (ptr.state == GAME_STATE)
	{
		// Player
		// 맵의 구조에 맞게 맵을 렌더링 해준다.
		for (y = 0; y < HEIGHT; y++) {
			for (x = 0; x < WIDTH - 1; x++) {
				switch (ns[y][x]) {
				case '#':
					iBit = 0;
					break;
				case 'O':
					iBit = 1;
					break;
				case '.':
					iBit = 2;
					break;
				case ' ':
					iBit = 3;
					break;
				}
				DrawBitmap(hdc, x * BW, y * BH, BW, BH, hBit[iBit]);
			}
		}

		// 플레이어 캐릭터를 표현한 원
		Ellipse(hdc, (nx * BW) + (BW / 2) + (BW / 2), (ny * BH) + (BH / 2) + (BH / 2), (nx * BW) - (BW / 2) + (BW / 2), (ny * BH) - (BH / 2) + (BH / 2));

		// Player 2
		// 상대편 맵
		for (y = 0; y < HEIGHT; y++) {
			for (x = 0; x < WIDTH - 1; x++) {
				switch (ps[y][x]) {
				case '#':
					iBit = 0;
					break;
				case 'O':
					iBit = 1;
					break;
				case '.':
					iBit = 2;
					break;
				case ' ':
					iBit = 3;
					break;
				}
				DrawBitmap(hdc, (x + 50) * BW, y * BH, BW, BH, hBit[iBit]);
			}
		}

		// 상대편 캐릭터를 원으로 표현
		Ellipse(hdc, ((px + 50) * BW) + (BW / 2) + (BW / 2), (py * BH) + (BH / 2) + (BH / 2),
			((px + 50) * BW) - (BW / 2) + (BW / 2), (py * BH) - (BH / 2) + (BH / 2));
	}

	// 텍스트
	wsprintf(Message, TEXT("SOKOBAN"));
	TextOut(hdc, 500, 10, Message, lstrlen(Message));
	wsprintf(Message, TEXT("Q:종료, R:다시하기"));
	TextOut(hdc, 500, 30, Message, lstrlen(Message));

	// 상태에 맞는 텍스트 출력
	if (ptr.state == WAIT_STATE)
		wsprintf(Message, TEXT("상대와 매치 대기중"));
	else if (ptr.state == GAME_STATE)
		wsprintf(Message, TEXT("게임 시작"));
	else if (ptr.state == LOSE_STATE)
		wsprintf(Message, TEXT("게임 패배"));
	else if (ptr.state == WIN_STATE)
		wsprintf(Message, TEXT("게임 승리"));

	TextOut(hdc, 500, 110, Message, lstrlen(Message));

}

// 비트맵 출력 함수
void DrawBitmap(HDC hdc,int x,int y, int _bx, int _by, HBITMAP hBit)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx,by;
	BITMAP bit;

	MemDC=CreateCompatibleDC(hdc);
	OldBitmap=(HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit,sizeof(BITMAP),&bit);
	bx = _bx;
	by = _by;

	BitBlt(hdc,x,y, bx, by, MemDC, bx / 2, by / 2, SRCCOPY);

	SelectObject(MemDC,OldBitmap);
	DeleteDC(MemDC);
}
