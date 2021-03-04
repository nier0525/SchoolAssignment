#include "MyFunction.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("Client");

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
		0, 0, 800, 600,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

User_Info user;
ROOMLIST roominfo;
DISPLAYSTATE display;
SocketPakage* p_socket;

HANDLE hThread;

POINT b_pos;
POINT m_pos;

char Text[MSGSIZE];
int count = 0;
int len = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	HDC backDC, DBDC;
	HBITMAP DB, oldDB;
	PAINTSTRUCT ps;
	static RECT rt;

	switch (iMessage) {
		// 초기화
	case WM_CREATE:
		SetTimer(hWnd, 0, 100, nullptr);

		ZeroMemory(&user, sizeof(User_Info));
		ZeroMemory(&p_socket, sizeof(p_socket));

		p_socket = new SocketPakage();
		display = LOGIN;	

		hThread = CreateThread(nullptr, 0, RecvThread, (LPVOID)p_socket, 0, nullptr);
		if (hThread == nullptr) {
			MessageBox(nullptr, "스레드 생성 실패.", "Thread Nullptr", MB_OK);
			KillTimer(hWnd, 0);
			CleanSocket(p_socket);
			delete p_socket;

			PostQuitMessage(0);
		}
		break;

		// 문자 입력
	case WM_CHAR:
		if (wParam != VK_RETURN && wParam != VK_ESCAPE)
		{
			switch (display) {
			case LOGIN:
				InputChar(wParam, user.name, &len, 20);
				break;

			case CHAT:
				InputChar(wParam, user.chat, &len, 50);
				break;
			}
		}
		break;

		// 키보드 입력
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_RETURN:
			switch (display) {
			case LOGIN:
				if (len > 1) {
					user.state = false;
					S_AddrChange(p_socket->send_addr);
					if (!RoomView(p_socket, roominfo)) {
						MessageBox(nullptr, "서버와 연결이 끊겼습니다.", "Disconneting", MB_OK);
						display = LOGIN;
					}
					else {
						display = ROOM;
					}
				}
				else {
					MessageBox(nullptr, "3 글자 이상 입력하세요", "Error", MB_OK);
				}
				break;

			case CHAT:
				if (user.chat[0] != NULL) {
					if (!SendToMessage(p_socket, user)) {
						ZeroMemory(&p_socket->TextArray, sizeof(p_socket->TextArray));
						p_socket->current = 0;

						Multicast_Group_Drop(p_socket->r_sock, p_socket->mreq);
						ZeroMemory(&p_socket->mreq, sizeof(ip_mreq));

						MessageBox(nullptr, "서버와 연결이 끊겼습니다.", "Disconneting", MB_OK);
						display = LOGIN;
						len = strlen(user.name);
						CloseHandle(hThread);
					}
				}
				break;
			}		
			break;

		case VK_ESCAPE:
			switch (display) {
			case LOGIN:
				KillTimer(hWnd, 0);
				CloseHandle(hThread);
				CleanSocket(p_socket);
				delete p_socket;

				PostQuitMessage(0);
				break;

			case ROOM:
				RoomSelect(p_socket, user, NULL);
				display = LOGIN;
				break;

			case CHAT:
				if (!roominfo.memberlist) {
					ZeroMemory(&user.chat, sizeof(user.chat));
					user.state = false;
					if (!JoinMessage(p_socket, user)) {
						MessageBox(nullptr, "서버와 연결이 끊겼습니다.", "Disconneting", MB_OK);
					}
					// CloseHandle(hThread);

					Multicast_Group_Drop(p_socket->r_sock, p_socket->mreq);
					ZeroMemory(&p_socket->mreq, sizeof(ip_mreq));

					S_AddrChange(p_socket->send_addr);
					if (!RoomView(p_socket, roominfo)) {
						MessageBox(nullptr, "서버와 연결이 끊겼습니다.", "Disconneting", MB_OK);
						display = LOGIN;
					}
					else {
						display = ROOM;
					}
					len = strlen(user.name);
					break;
				}
				else {				
					roominfo.memberlist = false;
				}
			}
			break;
		}
		break;

		// 왼쪽 마우스 입력
	case WM_LBUTTONDOWN:
		GetCursorPos(&m_pos);
		ScreenToClient(hWnd, &m_pos);

		switch (display) {
		case LOGIN:
			// Login Input
			if (CollisionEnterPoint(400, 340, m_pos.x, m_pos.y, 50, 20)) {
				if (len > 1) {
					user.state = false;
					S_AddrChange(p_socket->send_addr);
					if (!RoomView(p_socket, roominfo)) {
						MessageBox(nullptr, "서버와 연결이 끊겼습니다.", "Disconneting", MB_OK);
						display = LOGIN;
					}
					else {
						display = ROOM;
					}
				}
				else {
					MessageBox(nullptr, "3 글자 이상 입력하세요", "Error", MB_OK);
				}
			}
			break;

		case ROOM:
			// Room Select
			for (int i = 0; i < roominfo.roomsize; i++) {
				if (CollisionEnterPoint(400, (70 + (70 * i)), m_pos.x, m_pos.y, 200, 30)) {
					ZeroMemory(&user.chat, sizeof(user.chat));
					ZeroMemory(&p_socket->TextArray, sizeof(p_socket->TextArray));
					p_socket->current = 0;

					if (!RoomSelect(p_socket, user, i + 1)) {
						MessageBox(nullptr, "서버와 연결이 끊겼습니다.", "Disconneting", MB_OK);
						display = LOGIN;
					}
					else {
						user.room_num = i;
						display = CHAT;

						if (!Multicast_Group_Add(p_socket->r_sock, p_socket->mreq, user.room_ip)) {
							MessageBox(nullptr, "서버와 연결이 끊겼습니다.", "Disconneting", MB_OK);
							display = LOGIN;
						}
						else {

							S_AddrChange(p_socket->send_addr, user.room_ip);

							user.state = true;
							if (!JoinMessage(p_socket, user)) {
								Multicast_Group_Drop(p_socket->r_sock, p_socket->mreq);
								ZeroMemory(&p_socket->mreq, sizeof(ip_mreq));

								MessageBox(nullptr, "서버와 연결이 끊겼습니다.", "Disconneting", MB_OK);
								display = LOGIN;
							}
						}
					}
					break;
				}
			}

			// Back
			if (CollisionEnterPoint(400, 450, m_pos.x, m_pos.y, 50, 20)) {
				RoomSelect(p_socket, user, NULL);
				display = LOGIN;
			}
			break;

		case CHAT:
			// Chat Input
			if (CollisionEnterPoint(600, 500, m_pos.x, m_pos.y, 40, 20) && user.chat[0] != NULL) {
				if (!SendToMessage(p_socket, user)) {
					ZeroMemory(&user.chat, sizeof(user.chat));
					if (!SendToMessage(p_socket, user)) {
						MessageBox(nullptr, "서버와 연결이 끊겼습니다.", "Disconneting", MB_OK);
					}
					CloseHandle(hThread);

					Multicast_Group_Drop(p_socket->r_sock, p_socket->mreq);
					ZeroMemory(&p_socket->mreq, sizeof(ip_mreq));

					MessageBox(nullptr, "서버와 연결이 끊겼습니다.", "Disconneting", MB_OK);
					display = LOGIN;
					len = strlen(user.name);
				}
			}

			// Join UserList
			//if (CollisionEnterPoint(700, 25, m_pos.x, m_pos.y, 40, 20)) {
			//	if (!roominfo.memberlist) {
			//		roominfo.memberlist = true;
			//	}
			//	else {
			//		roominfo.memberlist = false;
			//	}
			//}

			// Back
			if (CollisionEnterPoint(700, 500, m_pos.x, m_pos.y, 40, 20)) {
				ZeroMemory(&user.chat, sizeof(user.chat));
				user.state = false;
				if (!JoinMessage(p_socket, user)) {
					MessageBox(nullptr, "서버와 연결이 끊겼습니다.", "Disconneting", MB_OK);
				}
				//CloseHandle(hThread);

				Multicast_Group_Drop(p_socket->r_sock, p_socket->mreq);
				ZeroMemory(&p_socket->mreq, sizeof(ip_mreq));

				S_AddrChange(p_socket->send_addr);
				if (!RoomView(p_socket, roominfo)) {
					MessageBox(nullptr, "서버와 연결이 끊겼습니다.", "Disconneting", MB_OK);
					display = LOGIN;
				}
				else {
					display = ROOM;
				}
				len = strlen(user.name);
			}
			break;
		}

		break;

		// 그래픽 출력
	case WM_PAINT:
		GetClientRect(hWnd, &rt);
		hdc = BeginPaint(hWnd, &ps);
		DBDC = CreateCompatibleDC(hdc);
		DB = CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
		oldDB = (HBITMAP)SelectObject(DBDC, DB);
		PatBlt(DBDC, 0, 0, rt.right, rt.bottom, WHITENESS);
		backDC = hdc;
		hdc = DBDC;
		DBDC = backDC;
		// --------------------------------------------------------------------------

		switch (display) {
		case LOGIN:
			LoginDisplay(hWnd, hdc, user);
			break;

		case ROOM:
			RoomSelectDisPlay(hWnd, hdc, user, roominfo);
			break;

		case CHAT:
			ChattingDisPlay(hWnd, hdc, user, roominfo, p_socket->current, p_socket->TextArray);
			break;
		}

		if (roominfo.memberlist) {
			MemberDIsPlay(hWnd, hdc, p_socket->count, p_socket->UserArray);
		}
		// ---------------------------------------------------------------------------
		backDC = hdc;
		hdc = DBDC;
		DBDC = backDC;
		BitBlt(hdc, 0, 0, rt.right, rt.bottom, DBDC, 0, 0, SRCCOPY);
		SelectObject(DBDC, oldDB);
		DeleteObject(DB);
		DeleteDC(DBDC);
		DeleteDC(backDC);

		EndPaint(hWnd, &ps);
		break;

		// 타이머
	case WM_TIMER:
		InvalidateRect(hWnd, &rt, false);
		break;

		// 정리
	case WM_DESTROY:
		ZeroMemory(&user.chat, sizeof(user.chat));
		user.state = false;
		if (!JoinMessage(p_socket, user)) {
			MessageBox(nullptr, "서버와 연결이 끊겼습니다.", "Disconneting", MB_OK);
		}

		KillTimer(hWnd, 0);
		CloseHandle(hThread);
		CleanSocket(p_socket);
		delete p_socket;

		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
