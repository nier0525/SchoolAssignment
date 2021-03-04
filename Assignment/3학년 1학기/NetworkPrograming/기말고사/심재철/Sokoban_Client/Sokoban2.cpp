#include "SocketFunction.h"
#include <windows.h>
#include "resource.h"

#define MAXSTAGE 3
#define BW 16
#define BH 16

// ���ӿ� �ʿ��� ����
// n = �ڽ� , p = ���
char ns[18][21];
char ps[18][21];

int nStage, pState;
int nx, ny;
int px, py;

// ��Ʈ�� ���ҽ�
HBITMAP hBit[5];

bool flag = false;

void Render(HDC hdc);
void DrawBitmap(HDC hdc,int x,int y, int _bx, int _by,HBITMAP hBit);

// Ŭ���̾�Ʈ ����
ClientInfo ptr;

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("Sokoban");

DWORD WINAPI ClientMain(LPVOID arg);
DWORD WINAPI RecvProcess(LPVOID arg);

// ������ �ʱ�ȭ
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	  ,LPSTR lpszCmdParam,int nCmdShow)
{
	// ������ �� ����
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

	// ������ ����
	hWnd=CreateWindow(lpszClass,lpszClass, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT,CW_USEDEFAULT,0,0,NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	
	// Ŭ���̾�Ʈ ������ ����
	CreateThread(nullptr, 0, ClientMain, nullptr, 0, nullptr);

	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

// Ŭ���̾�Ʈ ������
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

		// ���� ������ ����
		CreateThread(nullptr, 0, RecvProcess, nullptr, 0, 0);
	}

	bool Endflag = false;

	// ó���� �����޴ٰ� �۽��Ѵ�.
	PackPacket(ptr.s_buf, JOIN, ptr.size);
	retval = send(ptr.sock, ptr.s_buf, ptr.size, 0);
	if (retval == SOCKET_ERROR) err_quit("send");

	while (!Endflag)
	{
		// �б� �̺�Ʈ�� ���� ������ ���
		WaitForSingleObject(ptr.hWrite, INFINITE);
		ZeroMemory(ptr.s_buf, sizeof(ptr.s_buf));

		// ���� Ű���� ���� �۾� �з�
		switch (ptr.key)
		{
		// ���� ����
		case Q:
			// ���� �� ���� ���Ḧ �˸��� ���� �� ���۸� �۽����ش�.
			ZeroMemory(ptr.s_buf, sizeof(ptr.s_buf));
			retval = send(ptr.sock, ptr.s_buf, BUFSIZE, 0);
			if (retval == SOCKET_ERROR) err_quit("send");

			// �ݺ��� Ż��
			Endflag = true;
			break;

		// ���� ���� or �ٽ� ��Ī
		case R:
			// ���� ���� ��û
			if (ptr.state == GAME_STATE)
			{
				// ���� ������ ��û�ϴ� ��Ŷ�� �۽��Ѵ�.
				PackPacket(ptr.s_buf, RESET, ptr.size);
				retval = send(ptr.sock, ptr.s_buf, ptr.size, 0);
				if (retval == SOCKET_ERROR) err_quit("send");
			}
			// �ٽ� ��Ī�ϱ⸦ ��û
			if (ptr.state == WIN_STATE || ptr.state == LOSE_STATE)
			{
				// ������ ���� ��, �ٽ� ��Ī�ϱ⸦ ��û�ϴ� ��Ŷ�� �۽��Ѵ�.
				PackPacket(ptr.s_buf, JOIN, ptr.size);
				retval = send(ptr.sock, ptr.s_buf, ptr.size, 0);
				if (retval == SOCKET_ERROR) err_quit("send");
				
				ptr.state = WAIT_STATE;
				InvalidateRect(hWndMain, nullptr, false);
			}
			break;

		// ����Ű�� ���� �̵����� �������ش�.
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

	// ������ ���� �۾�
	for (int i = 0; i < 5; i++) {
		DeleteObject(hBit[i]);
	}

	closesocket(ptr.sock);
	WSACleanup();

	return 0;
}

// ���� ������
DWORD WINAPI RecvProcess(LPVOID arg)
{
	int retval;

	while (1)
	{
		// ���� ���̷� �� ���� ��Ŷ�� ���� �۾�
		if (!PacketRecv(ptr.sock, ptr.r_buf)) break;

		// �������ݿ� ���� �۾� �з�
		switch (GetProtocol(ptr.r_buf))
		{
		// ���� ��븦 ã�Ҵٴ� ��Ŷ�� �޾��� ��, ���� ���� ������ ��û�Ѵ�.
		case MATCH:
			PackPacket(ptr.s_buf, SETTING, ptr.size);
			retval = send(ptr.sock, ptr.s_buf, ptr.size, 0);
			if (retval == SOCKET_ERROR) err_quit("send");
			break;

		// ���õ� ���� ������ ���� ������ �����Ѵ�.
		// �׸��� ���� ���·� �������ش�.
		case MAPSETTING:
			UnPackPacket(ptr.r_buf, ns, ps, nStage, pState, nx, ny, px, py);
			px = nx;
			py = ny;

			ptr.state = GAME_STATE;
			InvalidateRect(hWndMain, nullptr, false);
			break;

		// ���� ���� �� ���� ���� ������ �������� ������Ʈ ���ش�.
		// ��밡 �����̰ų�, �ڽ��� ������ �� ��� ������Ʈ ��Ŷ�� �޴´�.
		case UPDATE:
			UnPackPacket(ptr.r_buf, ns, ps, nStage, pState, nx, ny, px, py);
			InvalidateRect(hWndMain, nullptr, false);
			break;

		// �¸����� ��� �¸� ���·� ����
		case WIN: 
			ptr.state = WIN_STATE;
			InvalidateRect(hWndMain, nullptr, false);
			break;

		// �й����� ��� �й� ���·� ����
		case LOSE:
			ptr.state = LOSE_STATE;
			InvalidateRect(hWndMain, nullptr, false);
			break;

		// �ٽ� �ٸ� ���� ��Ī �Ǳ⸦ ���ϸ� ����ġ ��Ŷ�� �۽��Ѵ�.
		// �� ��, �ٽ� ��� ���·� �����Ѵ�.
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

// ������ �۾� ������
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	// ���� ������
	HDC hdc, BackDC;
	HBITMAP Back, OldBack;

	PAINTSTRUCT ps;
	RECT crt;
	TCHAR Message[256];
				
	switch (iMessage) {
	case WM_CREATE:
		// ������ â�� ������ ������ �����Ѵ�.
		hWndMain = hWnd;
		SetRect(&crt, 0, 0, 1120, BH * 18);
		AdjustWindowRect(&crt, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);
		SetWindowPos(hWnd, NULL, 0, 0, crt.right - crt.left, crt.bottom - crt.top,
			SWP_NOMOVE | SWP_NOZORDER);

		// ��Ʈ�� �ҽ��� �ҷ��´�.
		for (int i = 0; i < 5; i++) {
			hBit[i] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_WALL + i));
		}
		return 0;

	case WM_PAINT:
		// ���� ���۸� �۾�
		GetClientRect(hWnd, &crt);
		hdc=BeginPaint(hWnd, &ps);

		BackDC = CreateCompatibleDC(hdc);
		SetGraphicsMode(hdc, GM_ADVANCED);
		Back = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
		OldBack = (HBITMAP)SelectObject(BackDC, Back);

		FillRect(BackDC, &crt, (HBRUSH)GetStockObject(WHITENESS));

		// ���� ������
		Render(BackDC);

		BitBlt(hdc, 0, 0, crt.right, crt.bottom, BackDC, 0, 0, SRCCOPY);
		SelectObject(BackDC, OldBack);

		DeleteObject(Back);
		DeleteDC(BackDC);

		EndPaint(hWnd, &ps);
		return 0;

	// Ű �Է¿� ���� ������ Ű���� �Է� ��, ���� �̺�Ʈ�� �����ش�.
	// ��, �Է��� ������ �������� Ű�� ������ �۽����ش�.
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

	// ������ ���� �� ���� �۾��� ���� ���� �̺�Ʈ�� ���ش�.
	case WM_DESTROY:
		ptr.key = Q;
		SetEvent(ptr.hWrite);

		PostQuitMessage(0);
		return 0;
	}

	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

// ������
void Render(HDC hdc)
{
	int x, y;
	int iBit;
	TCHAR Message[256];

	// ���� ������ ��� ���
	if (ptr.state == GAME_STATE)
	{
		// Player
		// ���� ������ �°� ���� ������ ���ش�.
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

		// �÷��̾� ĳ���͸� ǥ���� ��
		Ellipse(hdc, (nx * BW) + (BW / 2) + (BW / 2), (ny * BH) + (BH / 2) + (BH / 2), (nx * BW) - (BW / 2) + (BW / 2), (ny * BH) - (BH / 2) + (BH / 2));

		// Player 2
		// ����� ��
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

		// ����� ĳ���͸� ������ ǥ��
		Ellipse(hdc, ((px + 50) * BW) + (BW / 2) + (BW / 2), (py * BH) + (BH / 2) + (BH / 2),
			((px + 50) * BW) - (BW / 2) + (BW / 2), (py * BH) - (BH / 2) + (BH / 2));
	}

	// �ؽ�Ʈ
	wsprintf(Message, TEXT("SOKOBAN"));
	TextOut(hdc, 500, 10, Message, lstrlen(Message));
	wsprintf(Message, TEXT("Q:����, R:�ٽ��ϱ�"));
	TextOut(hdc, 500, 30, Message, lstrlen(Message));

	// ���¿� �´� �ؽ�Ʈ ���
	if (ptr.state == WAIT_STATE)
		wsprintf(Message, TEXT("���� ��ġ �����"));
	else if (ptr.state == GAME_STATE)
		wsprintf(Message, TEXT("���� ����"));
	else if (ptr.state == LOSE_STATE)
		wsprintf(Message, TEXT("���� �й�"));
	else if (ptr.state == WIN_STATE)
		wsprintf(Message, TEXT("���� �¸�"));

	TextOut(hdc, 500, 110, Message, lstrlen(Message));

}

// ��Ʈ�� ��� �Լ�
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
