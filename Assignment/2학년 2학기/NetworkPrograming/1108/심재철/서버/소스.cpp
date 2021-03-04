#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 9000
#define BUFSIZE    4096

#define PLAYER_COUNT 100

#define WAIT_MSG "매칭 대기 중 . . .\n"
#define INTRO_MSG "가위(0), 바위(1), 보(2)중 하나를 입력하세요 : "
#define WIN_MSG "이겼습니다.\n"
#define LOSE_MSG "졌습니다.\n"
#define NO_WIN_MSG "비겼습니다.\n"
#define ERROR_MSG "상대방이 나가서 잠시 후 다시 매칭을 합니다.\n"

enum RESULT
{
	_ERROR = -3,
	NODATA = -2,
	NO_WIN = -1,
	WIN = 1,
	LOSE
};

enum PROTOCOL
{
	WAIT = -1,
	INTRO = 1,
	GAME_VALUE,
	GAME_RESULT
};

struct _ClientInfo
{
	SOCKET sock;
	SOCKADDR_IN addr;
	char buf[BUFSIZE];
	int game_value;
	_ClientInfo* part;
};

// 전역 변수
_ClientInfo* ClientInfo[PLAYER_COUNT];
int Count = 0;

CRITICAL_SECTION cs;

void err_quit(const char *msg);
void err_display(const char *msg);

int recvn(SOCKET s, char *buf, int len, int flags);

int who_win(int first, int second);

bool PacketRecv(SOCKET _sock, char* _buf);
PROTOCOL GetProtocol(const char* _ptr);
int PackPacket(char* _buf, PROTOCOL _protocol, const char* _str1);
int PackPacket(char* _buf, PROTOCOL _protocol, int _result, const char* _str1);

void UnPackPacket(const char* _buf, int& _data);

_ClientInfo* AddClientInfo(SOCKET _sock, SOCKADDR_IN _addr);
void RemoveClientInfo(_ClientInfo* _ptr);
bool MatchPartner(_ClientInfo*);

DWORD WINAPI Client_Thread(LPVOID arg);

int main(int argc, char *argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	InitializeCriticalSection(&cs);
	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	int size;

	while (1)
	{
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}

		_ClientInfo* ptr = AddClientInfo(client_sock, clientaddr);

		HANDLE hThread = CreateThread(NULL, 0, Client_Thread, ptr, 0, NULL);

		if (hThread == NULL)
		{
			RemoveClientInfo(ptr);
			continue;
		}
		else
		{
			CloseHandle(hThread);
		}
	}

	// closesocket()
	closesocket(listen_sock);
	DeleteCriticalSection(&cs);
	// 윈속 종료
	WSACleanup();
	return 0;
}

DWORD WINAPI Client_Thread(LPVOID arg) {
	_ClientInfo* ptr = (_ClientInfo*)arg;

	PROTOCOL protocol;
	RESULT result;

	int retval;
	int size;

	bool endflag;

	while (1) {
		endflag = true;

		if (Count % 2 == 0) {
			if (!MatchPartner(ptr) && ptr->part == nullptr) {
				Sleep(500);
				continue;
			}
			else {
				ptr->part->game_value = NODATA;
				size = PackPacket(ptr->buf, INTRO, INTRO_MSG);
			}
		}
		else {
			ptr->part = nullptr;
			size = PackPacket(ptr->buf, WAIT, WAIT_MSG);
		}

		retval = send(ptr->sock, ptr->buf, size, 0);
		if (retval == SOCKET_ERROR) {
			err_display("intro or wait send");
			break;
		}

		if (!PacketRecv(ptr->sock, ptr->buf)) {
			RemoveClientInfo(ptr);
			return -1;
		}

		protocol = GetProtocol(ptr->buf);

		switch (protocol) {
		case WAIT:
			while (1) {
				if (Count % 2 == 0) {
					break;
				}
				else {
					size = PackPacket(ptr->buf, WAIT, WAIT_MSG);
					retval = send(ptr->sock, ptr->buf, size, 0);
					if (retval == SOCKET_ERROR) {
						err_display("intro or wait send");
						RemoveClientInfo(ptr);
						return -1;
					}

					if (!PacketRecv(ptr->sock, ptr->buf)) {
						RemoveClientInfo(ptr);
						return -1;
					}
				}
				Sleep(1000);
			}
			continue;

		case GAME_VALUE:
			UnPackPacket(ptr->buf, ptr->game_value);
			while (1) {
				if (ptr->part->game_value != NODATA) {
					break;
				}
				Sleep(100);
			}
			break;
		}

		result = (RESULT)who_win(ptr->game_value, ptr->part->game_value);
		Sleep(200);

		char msg[128];
		ZeroMemory(msg, sizeof(msg));

		switch (result) {
		case _ERROR:
			strcpy(msg, ERROR_MSG);
			ptr->part = nullptr;
			endflag = false;
			break;

		case NO_WIN:
			strcpy(msg, NO_WIN_MSG);
			endflag = false;
			break;

		case WIN:
			strcpy(msg, WIN_MSG);
			break;

		case LOSE:
			strcpy(msg, LOSE_MSG);
			break;
		}

		size = PackPacket(ptr->buf, GAME_RESULT, result, msg);
		retval = send(ptr->sock, ptr->buf, size, 0);
		if (retval == SOCKET_ERROR) {
			break;
		}

		if (endflag) {
			break;
		}
	}

	if (!PacketRecv(ptr->sock, ptr->buf)) {
		RemoveClientInfo(ptr);
	}
	return 0;
}

bool MatchPartner(_ClientInfo* _ptr)
{
	if (_ptr->part != nullptr) {
		return false;
	}

	EnterCriticalSection(&cs);
	for (int i = 0; i < Count; i++)
	{
		if (_ptr != ClientInfo[i] && ClientInfo[i]->part == nullptr)
		{
			ClientInfo[i]->part = _ptr;
			_ptr->part = ClientInfo[i];
			LeaveCriticalSection(&cs);
			return true;
		}
	}

	LeaveCriticalSection(&cs);
	return false;
}

void err_quit(const char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(const char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

int who_win(int first, int second)
{
	if (second < NODATA)
		return _ERROR;
	else if (first == second)
		return NO_WIN;
	else if (first % 3 == (second + 1) % 3)
		return WIN;
	else
		return LOSE;
}

bool PacketRecv(SOCKET _sock, char* _buf)
{
	int size;

	int retval = recvn(_sock, (char*)&size, sizeof(size), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv error()");
		return false;
	}
	else if (retval == 0)
	{
		return false;
	}

	retval = recvn(_sock, _buf, size, 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv error()");
		return false;

	}
	else if (retval == 0)
	{
		return false;
	}

	return true;
}

PROTOCOL GetProtocol(const char* _ptr)
{
	PROTOCOL protocol;
	memcpy(&protocol, _ptr, sizeof(PROTOCOL));

	return protocol;
}

int PackPacket(char* _buf, PROTOCOL _protocol, const char* _str1)
{
	int size = 0;
	char* ptr = _buf;
	int strsize1 = strlen(_str1);

	ptr = ptr + sizeof(size);

	memcpy(ptr, &_protocol, sizeof(_protocol));
	ptr = ptr + sizeof(_protocol);
	size = size + sizeof(_protocol);

	memcpy(ptr, &strsize1, sizeof(strsize1));
	ptr = ptr + sizeof(strsize1);
	size = size + sizeof(strsize1);

	memcpy(ptr, _str1, strsize1);
	ptr = ptr + strsize1;
	size = size + strsize1;

	ptr = _buf;
	memcpy(ptr, &size, sizeof(size));

	size = size + sizeof(size);

	return size;
}

int PackPacket(char* _buf, PROTOCOL _protocol, int _result, const char* _str1)
{
	int size = 0;
	char* ptr = _buf;
	int strsize1 = strlen(_str1);

	ptr = ptr + sizeof(size);

	memcpy(ptr, &_protocol, sizeof(_protocol));
	ptr = ptr + sizeof(_protocol);
	size = size + sizeof(_protocol);

	memcpy(ptr, &_result, sizeof(_result));
	ptr = ptr + sizeof(_result);
	size = size + sizeof(_result);

	memcpy(ptr, &strsize1, sizeof(strsize1));
	ptr = ptr + sizeof(strsize1);
	size = size + sizeof(strsize1);

	memcpy(ptr, _str1, strsize1);
	ptr = ptr + strsize1;
	size = size + strsize1;

	ptr = _buf;
	memcpy(ptr, &size, sizeof(size));

	size = size + sizeof(size);

	return size;
}

void UnPackPacket(const char* _buf, int& _data)
{
	const char* ptr = _buf + sizeof(PROTOCOL);

	memcpy(&_data, ptr, sizeof(_data));
	ptr = ptr + sizeof(_data);

}

_ClientInfo* AddClientInfo(SOCKET _sock, SOCKADDR_IN _addr)
{
	EnterCriticalSection(&cs);
	_ClientInfo* ptr = new _ClientInfo;
	ZeroMemory(ptr, sizeof(_ClientInfo));
	ptr->sock = _sock;
	memcpy(&ptr->addr, &_addr, sizeof(SOCKADDR_IN));
	ptr->game_value = NODATA;
	ptr->part = nullptr;

	ClientInfo[Count++] = ptr;

	LeaveCriticalSection(&cs);

	printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));

	return ptr;
}

void RemoveClientInfo(_ClientInfo* _ptr)
{
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(_ptr->addr.sin_addr), ntohs(_ptr->addr.sin_port));

	EnterCriticalSection(&cs);
	for (int i = 0; i < Count; i++)
	{
		if (ClientInfo[i] == _ptr)
		{
			closesocket(ClientInfo[i]->sock);

			/*if (ClientInfo[i]->part != NULL)
			{
				ClientInfo[i]->part->part = NULL;
			}*/
			delete ClientInfo[i];

			for (int j = i; j < Count - 1; j++)
			{
				ClientInfo[j] = ClientInfo[j + 1];
			}

			break;
		}
	}

	Count--;

	LeaveCriticalSection(&cs);
}
