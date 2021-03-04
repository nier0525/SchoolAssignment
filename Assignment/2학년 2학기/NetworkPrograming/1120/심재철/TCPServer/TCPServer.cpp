#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 9000
#define BUFSIZE    4096

#define PLAYER_COUNT 100

#define INTRO_MSG "가위(0), 바위(1), 보(2)중 하나를 입력하세요:"
#define WAIT_MSG "상대의 응답을 기다리고 있습니다. . .\n"
#define WIN_MSG "이겼습니다.\n"
#define LOSE_MSG "졌습니다.\n"
#define NO_WIN_MSG "비겼습니다\n"

enum STATE {
	INIT_STATE = 1,
	INTRO_STATE,
	GAME_VALUE_STATE,
	WAIT_STATE,
	GAME_RESULT_STATE,

	REMATCH_STATE
};

enum RESULT
{	
	NODATA=-1,
	WIN = 1,
	LOSE,
	NO_WIN
};

enum PROTOCOL
{
	INTRO=1,
	GAME_VALUE,
	WAIT,
	GAME_RESULT
};

struct _ClientInfo
{
	SOCKET sock;
	SOCKADDR_IN addr;
	char buf[BUFSIZE];
	int game_value;
	_ClientInfo* part;

	STATE state;
	HANDLE hThread;
	HANDLE hEvent;

	bool game_value_flag;
	bool no_win_flag;
	bool end_flag;
};

_ClientInfo* ClientInfo[PLAYER_COUNT];
int Count = 0;

HANDLE hThread[PLAYER_COUNT];
int Thread_Count = 0;

CRITICAL_SECTION cs;

SOCKET socket_init();

void err_quit(const char *msg);
void err_display(const char *msg);

int recvn(SOCKET s, char *buf, int len, int flags);

RESULT who_win(int first, int second);

bool PacketRecv(SOCKET _sock, char* _buf);
PROTOCOL GetProtocol(const char* _ptr);
int PackPacket(char* _buf, PROTOCOL _protocol, const char* _str1);
int PackPacket(char* _buf, PROTOCOL _protocol, RESULT _result, const char* _str1);

void UnPackPacket(const char* _buf, int& _data);

_ClientInfo* AddClientInfo(SOCKET _sock, SOCKADDR_IN _addr);
void RemoveClientInfo(_ClientInfo* _ptr);

bool AddThread(LPTHREAD_START_ROUTINE, _ClientInfo*);
void DeleteThread(_ClientInfo*);

bool MatchPartner(_ClientInfo*);

DWORD WINAPI Client_Thread(LPVOID);
DWORD CALLBACK Wait_Thread(LPVOID);
DWORD CALLBACK Dump_Thread(LPVOID);

int main(int argc, char *argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return 1;

	SOCKET listen_sock = socket_init();

	InitializeCriticalSection(&cs);

	// 데이터 통신에 사용할 변수
	hThread[Thread_Count++] = CreateThread(NULL, 0, Dump_Thread, NULL, 0, NULL);
	if (hThread[Thread_Count - 1] == NULL) {
		return -1;
	}

	HANDLE hWait = CreateThread(NULL, 0, Wait_Thread, NULL, 0, NULL);
	if (hWait == NULL) {
		return -1;
	}
	else {
		CloseHandle(hWait);
	}

	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;	
	int size;

	while (1)
	{
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr,
			&addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}

		_ClientInfo* ptr = AddClientInfo(client_sock, clientaddr);

		MatchPartner(ptr);

		if (!AddThread(Client_Thread, ptr)) {
			RemoveClientInfo(ptr);
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

	int size;
	int retval;
	int recived;

	RESULT result;
	PROTOCOL protocol;

	while (1) {
		ptr->end_flag = false;

		switch (ptr->state)
		{
		case INIT_STATE:
			ptr->state = INTRO_STATE;
			break;

		case INTRO_STATE:
			size = PackPacket(ptr->buf, INTRO, INTRO_MSG);
			retval = send(ptr->sock, ptr->buf, size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("intro send");
				ptr->end_flag = true;
				ptr->state = REMATCH_STATE;
			}

			ptr->state = GAME_VALUE_STATE;
			break;

		case GAME_VALUE_STATE:
			if (!PacketRecv(ptr->sock, ptr->buf)) {
				err_display("game value recv");
				ptr->end_flag = true;
				ptr->state = REMATCH_STATE;
			}

			ResetEvent(ptr->hEvent);
			protocol = GetProtocol(ptr->buf);

			switch (protocol) {
			case GAME_VALUE:
				UnPackPacket(ptr->buf, ptr->game_value);
				ptr->game_value_flag = false;

				if (ptr->game_value != NODATA) {
					size = PackPacket(ptr->buf, WAIT, WAIT_MSG);
					retval = send(ptr->sock, ptr->buf, size, 0);
					if (retval == SOCKET_ERROR) {
						err_display("intro send");
						ptr->end_flag = true;
						ptr->state = REMATCH_STATE;
					}
				}
				break;
			}

			ptr->state = WAIT_STATE;
			break;

		case WAIT_STATE:
			if (ptr->part != nullptr && ptr->part->game_value != NODATA) {
				SetEvent(ptr->part->hEvent);
				SetEvent(ptr->hEvent);
			}

			recived = WaitForSingleObject(ptr->hEvent, INFINITE);
			if (recived != WAIT_OBJECT_0) {
				ptr->end_flag = true;
			}

			ptr->state = GAME_RESULT_STATE;
			break;

		case GAME_RESULT_STATE:
			EnterCriticalSection(&cs);
			result = who_win(ptr->game_value, ptr->part->game_value);
			LeaveCriticalSection(&cs);

			ResetEvent(ptr->hEvent);

			switch (result) {
			case NO_WIN:
				ptr->no_win_flag = true;

				size = PackPacket(ptr->buf, GAME_RESULT, NO_WIN, NO_WIN_MSG);
				retval = send(ptr->sock, ptr->buf, size, 0);
				if (retval == SOCKET_ERROR) {
					err_display("game result send");
					ptr->end_flag = true;
				}
				break;

			case WIN:
				ptr->end_flag = true;

				size = PackPacket(ptr->buf, GAME_RESULT, WIN, WIN_MSG);
				retval = send(ptr->sock, ptr->buf, size, 0);
				if (retval == SOCKET_ERROR) {
					err_display("game result send");
				}
				break;

			case LOSE:
				ptr->end_flag = true;

				size = PackPacket(ptr->buf, GAME_RESULT, LOSE, LOSE_MSG);
				retval = send(ptr->sock, ptr->buf, size, 0);
				if (retval == SOCKET_ERROR) {
					err_display("game result send");
				}
				break;
			}

			ptr->game_value_flag = true;

			if (ptr->part != nullptr && ptr->part->game_value_flag) {
				SetEvent(ptr->part->hEvent);
				SetEvent(ptr->hEvent);
			}
			else if (ptr->part == nullptr) {
				SetEvent(ptr->hEvent);
				ptr->end_flag = true;
			}

			recived = WaitForSingleObject(ptr->hEvent, INFINITE);
			if (recived != WAIT_OBJECT_0) {
				ptr->end_flag = true;
			}

			if (ptr->no_win_flag) {
				ptr->game_value = NODATA;
				ptr->no_win_flag = false;

				ptr->state = GAME_VALUE_STATE;
				Sleep(0);
			}
			break;
		}

		if (ptr->end_flag) {
			break;
		}
	}

	//printf("exit thread\n");
	return 0;
}

DWORD CALLBACK Wait_Thread(LPVOID arg) {
	while (1) {
		int index = WaitForMultipleObjects(Thread_Count, hThread, false, INFINITE);
		index -= WAIT_OBJECT_0;

		if (index == 0) {
			hThread[0] = CreateThread(NULL, 0, Dump_Thread, NULL, 0, NULL);
			if (hThread[0] == NULL) {
				return 0;
			}
			continue;
		}

		else {
			for (int i = 0; i < Count; i++) {
				_ClientInfo* ptr = ClientInfo[i];

				if (ptr->hThread == hThread[index]) {
					switch (ptr->state) {
					case INTRO_STATE:
					case GAME_VALUE_STATE:
					case WAIT_STATE:
					case REMATCH_STATE:
						_ClientInfo* part;
						part = ptr->part;

						DeleteThread(ptr);
						RemoveClientInfo(ptr);

						if (MatchPartner(part)) {
							if (part->part != nullptr && part->part->game_value != NODATA) {
								SetEvent(part->part->hEvent);
								SetEvent(part->hEvent);
							}
						}
						break;

					case GAME_RESULT_STATE:
						DeleteThread(ptr);
						RemoveClientInfo(ptr);
						break;
					}
				}
			}
		}
	}

	return 0;
}

DWORD CALLBACK Dump_Thread(LPVOID) {
	while (1) {
		if (hThread[1] != nullptr) {
			break;
		}

		Sleep(0);
	}
	return 0;
}

bool MatchPartner(_ClientInfo* _ptr)
{
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

SOCKET socket_init() {
	int retval;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr,
		sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	return listen_sock;
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

RESULT who_win(int first, int second)
{
	if (first == second)
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

int PackPacket(char* _buf, PROTOCOL _protocol, RESULT _result, const char* _str1)
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

	ptr->state = INIT_STATE;
	ptr->hThread = nullptr;

	ptr->hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	ptr->game_value_flag = false;
	ptr->no_win_flag = false;
	ptr->end_flag = false;

	ClientInfo[Count++] = ptr;

	LeaveCriticalSection(&cs);

	printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));

	return ptr;
}

void RemoveClientInfo(_ClientInfo* _ptr)
{
	printf("\n[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(_ptr->addr.sin_addr), ntohs(_ptr->addr.sin_port));

	EnterCriticalSection(&cs);

	for (int i = 0; i < Count; i++)
	{
		if (ClientInfo[i] == _ptr)
		{
			closesocket(ClientInfo[i]->sock);
			CloseHandle(ClientInfo[i]->hEvent);

			if (ClientInfo[i]->part != NULL)
			{
				ClientInfo[i]->part->part = NULL;
			}
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

bool AddThread(LPTHREAD_START_ROUTINE process, _ClientInfo* ptr) {
	EnterCriticalSection(&cs);

	ptr->hThread = CreateThread(NULL, 0, process, ptr, 0, NULL);
	if (ptr->hThread == NULL) {
		LeaveCriticalSection(&cs);
		return false;
	}

	hThread[Thread_Count++] = ptr->hThread;

	LeaveCriticalSection(&cs);
	return true;
}

void DeleteThread(_ClientInfo* ptr) {
	EnterCriticalSection(&cs);

	for (int i = 1; i < Thread_Count; i++) {
		if (hThread[i] == ptr->hThread) {
			CloseHandle(ptr->hThread);
			ptr->hThread = nullptr;

			for (int j = i; j < Thread_Count - 1; j++) {
				hThread[j] = hThread[j + 1];
			}
			break;
		}
	}

	hThread[Thread_Count - 1] = nullptr;
	Thread_Count--;

	LeaveCriticalSection(&cs);
}

