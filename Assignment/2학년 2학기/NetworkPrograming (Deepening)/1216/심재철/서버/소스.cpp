#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <stdio.h>

#define SERVER_PORT 6470
#define BUFSIZE 4096
#define MSGSIZE 512

#define ROOM_MAX 4

#define WAIT_MSG "상대방의 접속을 기다리고 있습니다."
#define INTRO_MSG "\n상대가 입장했습니다.\n31 를 먼저 부르는 사람이 패배합니다.\n상대가 부른 수에서 최대 + 3 까지 부를 수 있습니다."
#define WIN_MSG "승리하셨습니다."
#define LOSE_MSG "패배하셨습니다."
#define GIVEUP_MSG "상대가 게임을 포기하였습니다."
#define ERROR_MSG "잘못된 입력입니다."

enum STATE { 
	S_INIT = 1, 
	S_WAIT, 
	S_INTRO, 
	S_CLIENTTRUN,
	S_WAITTRUN,
	S_RESULT
};

enum PROTOCOL { 
	WAIT = -1, 
	INTRO = 1,
	ANSWER_INFO,
	SELECT_NUM,
	ERROR_NUM,
	ANSWER_RESULT
};

enum RESULT {
	GIVEUP = -1, 
	WIN = 1, 
	LOSE
};

struct _ClientInfo;
struct _RoomInfo;

struct _ClientInfo {
	SOCKET sock;
	SOCKADDR_IN addr;
	char buf[BUFSIZE];

	STATE state;
	
	HANDLE hGameTrun;
	HANDLE hThread;

	_RoomInfo* room;
};

struct _RoomInfo {
	_ClientInfo* client[4];
	int count;

	int order;
	int answer;

	HANDLE hHost;
	HANDLE hStart;

	bool count_full;
	bool no_entry;
	bool endflag;
};

_ClientInfo* client_info[100];
int client_count = 0;

HANDLE Thread[100];
int thread_count = 0;

_RoomInfo* roominfo[50];
int room_count = 0;

CRITICAL_SECTION cs;

void err_quit(const char *msg);																			// 에러 함수 ( 종료 )
void err_display(const char *msg);																		// 에러 함수 ( 출력 )

SOCKET socket_init();

_ClientInfo* AddClientInfo(SOCKET sock, SOCKADDR_IN addr);
_ClientInfo* SearchClientInfo(HANDLE arg);
void RemoveClientInfo(_ClientInfo* ptr);

bool AddThread(LPTHREAD_START_ROUTINE arg, _ClientInfo* ptr);
void RemoveThread(HANDLE ptr);

void CreateRoom(_ClientInfo* ptr);
void RemoveRoom(_ClientInfo* ptr);

bool MatchRoom(_ClientInfo* ptr);
bool OutRoom(_ClientInfo* ptr);

int recvn(SOCKET s, char *buf, int len, int flags);												// 사용자 지정 Recv 함수
bool PacketRecv(SOCKET _sock, char* _buf);													// Paking 을 위한 Recv 함수
int GetProtocol(const char* _ptr);

int PackPacket(char* _buf, int _protocol, const char* _str);
int PackPacket(char* _buf, int _protocol, int result, const char* _str);

void UnPackPacket(const char* _buf, int& _data);

DWORD WINAPI ClientProcess(LPVOID arg);
DWORD CALLBACK WaitProcess(LPVOID arg);

int main() {
	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return -1;

	SOCKET listen_sock = socket_init();

	InitializeCriticalSection(&cs);

	SOCKET client_sock;
	SOCKADDR_IN client_addr;
	int addrlen;

	Thread[thread_count++] = CreateEvent(nullptr, false, false, nullptr);
	CreateThread(nullptr, 0, WaitProcess, nullptr, 0, nullptr);

	while (1) {
		// Accept
		addrlen = sizeof(SOCKADDR_IN);
		client_sock = accept(listen_sock, (SOCKADDR*)&client_addr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("Accept");
			continue;
		}

		_ClientInfo* ptr = AddClientInfo(client_sock, client_addr);
		if (!MatchRoom(ptr)) {
			CreateRoom(ptr);
		}

		if (!AddThread(ClientProcess, ptr)) {
			if (!OutRoom(ptr)) {
				RemoveRoom(ptr);
			}
			RemoveClientInfo(ptr);
		}
		else {
			SetEvent(Thread[0]);
		}
	}

	DeleteCriticalSection(&cs);
	closesocket(listen_sock);
	WSACleanup();
	return 0;
}

DWORD WINAPI ClientProcess(LPVOID arg) {
	_ClientInfo* ptr = (_ClientInfo*)arg;
	char msg[MSGSIZE];

	int retval;
	int size;

	PROTOCOL protocol;
	RESULT result;

	bool flag = false;

	while (1) {
		switch (ptr->state) {
		case S_INIT:
			ptr->state = S_WAIT;
			break;

		case S_WAIT:
			size = PackPacket(ptr->buf, (PROTOCOL)WAIT, WAIT_MSG);
			retval = send(ptr->sock, ptr->buf, size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("wait send");
				flag = true;
			}
			ptr->state = S_INTRO;
			break;

		case S_INTRO:
			if (ptr->room->client[0] == ptr) {
				WaitForSingleObject(ptr->room->hHost, INFINITE);
			}
			else {
				WaitForSingleObject(ptr->room->hStart, INFINITE);
			}

			if (ptr->room->count > 1) {
				size = PackPacket(ptr->buf, (PROTOCOL)INTRO, INTRO_MSG);
				retval = send(ptr->sock, ptr->buf, size, 0);
				if (retval == SOCKET_ERROR) {
					if (!OutRoom(ptr)) {
						RemoveRoom(ptr);
					}
					ResetEvent(ptr->room->hHost);
					err_display("intro send");
					flag = true;
				}
			}

			SetEvent(ptr->room->hStart);

			if (ptr->room->count > 1) {
				if (ptr->room->client[0] == ptr) {
					ptr->state = S_CLIENTTRUN;
				}
				else {
					ptr->state = S_WAITTRUN;
				}
			}
			else {
				ResetEvent(ptr->room->hStart);
			}
			break;

		case S_WAITTRUN:
			WaitForSingleObject(ptr->hGameTrun, INFINITE);
			if (ptr->room->answer >= 31) {
				ptr->state = S_RESULT;
			}
			else if (ptr->room->count <= 1) {
				ptr->state = S_RESULT;
			}
			else {
				ptr->state = S_CLIENTTRUN;
			}
			break;

		case S_CLIENTTRUN:
			sprintf(msg, "%d", ptr->room->answer);
			size = PackPacket(ptr->buf, (PROTOCOL)ANSWER_INFO, msg);
			retval = send(ptr->sock, ptr->buf, size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("Answer Info Send");
				flag = true;
			}

			if (!PacketRecv(ptr->sock, ptr->buf)) {
				flag = true;
			}

			switch ((PROTOCOL)GetProtocol(ptr->buf)) {
			case SELECT_NUM:
				int temp;
				UnPackPacket(ptr->buf, temp);
				if (temp > ptr->room->answer && temp <= (ptr->room->answer + 3)) {
					ptr->room->answer = temp;
					ptr->state = S_RESULT;
				}
				else {
					size = PackPacket(ptr->buf, (PROTOCOL)ERROR_NUM, ERROR_MSG);
					retval = send(ptr->sock, ptr->buf, size, 0);
					if (retval == SOCKET_ERROR) {
						err_display("Select Error Send");
						flag = true;
					}
				}
				break;
			}
			break;

		case S_RESULT:
			ptr->room->order++;
			if (ptr->room->order >= ptr->room->count) {
				ptr->room->order = 0;
			}

			SetEvent(ptr->room->client[ptr->room->order]->hGameTrun);

			if (ptr->room->count <= 1) {
				size = PackPacket(ptr->buf, (PROTOCOL)ANSWER_RESULT, (RESULT)GIVEUP, GIVEUP_MSG);
				retval = send(ptr->sock, ptr->buf, size, 0);
				if (retval == SOCKET_ERROR) {
					err_display("Give Up Send");
					flag = true;
				}

				flag = true;
			}
			else {
				if (ptr->room->answer >= 15) {
					ptr->room->no_entry = true;
				}

				if (ptr->room->answer >= 31) {
					if (!ptr->room->endflag) {
						size = PackPacket(ptr->buf, (PROTOCOL)ANSWER_RESULT, (RESULT)LOSE, LOSE_MSG);
						ptr->room->endflag = true;
					}
					else {
						size = PackPacket(ptr->buf, (PROTOCOL)ANSWER_RESULT, (RESULT)WIN, WIN_MSG);
					}
					retval = send(ptr->sock, ptr->buf, size, 0);
					if (retval == SOCKET_ERROR) {
						err_display("Answer Result Send");
						flag = true;
					}

					flag = true;
				}
				else {
					ptr->state = S_WAITTRUN;
				}
			}
			break;
		}

		if (flag) {
			if (!PacketRecv(ptr->sock, ptr->buf)) {
				break;
			}
		}
	}

	return 0;
}

DWORD CALLBACK WaitProcess(LPVOID arg) {
	while (1) {
		int index = WaitForMultipleObjects(thread_count, Thread, false, INFINITE);
		index -= WAIT_OBJECT_0;
		EnterCriticalSection(&cs);
		if (index == 0) {
			LeaveCriticalSection(&cs);
			continue;
		}

		_ClientInfo* ptr = SearchClientInfo(Thread[index]);
		if (ptr == nullptr) {
			RemoveThread(Thread[index]);
			LeaveCriticalSection(&cs);
			continue;
		}

		RemoveThread(Thread[index]);
		ptr->hThread = nullptr;

		switch (ptr->state) {
		case S_INTRO:
			RemoveClientInfo(ptr);
			break;

		case S_CLIENTTRUN:
			OutRoom(ptr);
			if (ptr->room->order >= ptr->room->count) {
				ptr->room->order = 0;
			}

			SetEvent(ptr->room->client[ptr->room->order]->hGameTrun);
			RemoveClientInfo(ptr);
			break;

		default:
			if (!OutRoom(ptr)) {
				RemoveRoom(ptr);
			}
			RemoveClientInfo(ptr);
			break;
		}

		LeaveCriticalSection(&cs);
	}
	return 0;
}

_ClientInfo* AddClientInfo(SOCKET sock, SOCKADDR_IN addr) {
	EnterCriticalSection(&cs);
	_ClientInfo* ptr = new _ClientInfo();
	ZeroMemory(ptr, sizeof(_ClientInfo));
	ptr->sock = sock;
	memcpy(&ptr->addr, &addr, sizeof(SOCKADDR_IN));
	ptr->state = S_INIT;
	ptr->room = nullptr;

	ptr->hThread = nullptr;
	ptr->hGameTrun = CreateEvent(nullptr, false, false, nullptr);

	client_info[client_count++] = ptr;
	LeaveCriticalSection(&cs);

	printf("\n[TCP 서버] 클라이언트 접속 : IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));

	return ptr;
}

_ClientInfo* SearchClientInfo(HANDLE arg) {
	EnterCriticalSection(&cs);
	_ClientInfo* ptr = nullptr;

	for (int i = 0; i < client_count; i++) {
		if (client_info[i]->hThread == arg) {
			ptr = client_info[i];
			break;
		}
	}

	LeaveCriticalSection(&cs);
	return ptr;
}

void RemoveClientInfo(_ClientInfo* ptr) {
	printf("\n[TCP 서버] 클라이언트 종료 : IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));

	EnterCriticalSection(&cs);
	for (int i = 0; i < client_count; i++) {
		if (client_info[i] == ptr) {
			closesocket(client_info[i]->sock);
			CloseHandle(client_info[i]->hGameTrun);

			delete client_info[i];

			for (int j = i; j < client_count - 1; j++) {
				client_info[j] = client_info[j + 1];
			}
			break;
		}
	}

	client_count--;
	LeaveCriticalSection(&cs);
}

bool AddThread(LPTHREAD_START_ROUTINE arg, _ClientInfo* ptr) {
	EnterCriticalSection(&cs);
	ptr->hThread = CreateThread(nullptr, 0, arg, ptr, 0, nullptr);
	if (ptr->hThread == nullptr) {
		LeaveCriticalSection(&cs);
		return false;
	}

	Thread[thread_count++] = ptr->hThread;

	LeaveCriticalSection(&cs);
	return true;
}

void RemoveThread(HANDLE ptr) {
	EnterCriticalSection(&cs);
	for (int i = 0; i < thread_count; i++) {
		if (Thread[i] == ptr) {
			CloseHandle(ptr);

			for (int j = i; j < thread_count - 1; j++) {
				Thread[j] = Thread[j + 1];
			}

			Thread[thread_count - 1] = nullptr;
			thread_count--;
			break;
		}
	}
	LeaveCriticalSection(&cs);
}

void CreateRoom(_ClientInfo* ptr) {
	EnterCriticalSection(&cs);
	_RoomInfo* room = new _RoomInfo();

	room->count_full = false;
	room->no_entry = false;
	room->endflag = false;

	room->answer = 0;
	room->count = 0;
	room->order = 0;

	room->hHost = CreateEvent(nullptr, true, false, nullptr);
	room->hStart = CreateEvent(nullptr, true, false, nullptr);

	room->client[room->count++] = ptr;
	ptr->room = room;

	printf("\n[ %s / %d Creates In The Room ]\n", inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));
	roominfo[room_count++] = room;
	LeaveCriticalSection(&cs);
}

void RemoveRoom(_ClientInfo* ptr) {
	EnterCriticalSection(&cs);
	for (int i = 0; i < room_count; i++) {
		if (roominfo[i] == ptr->room) {
			printf("\n[ %s / %d Deletes In The Room ]\n", inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));
			CloseHandle(roominfo[i]->hStart);
			delete roominfo[i];
			for (int n = i; n < room_count - 1; n++) {
				roominfo[n] = roominfo[n + 1];
			}
			room_count--;
			break;
		}
	}
	LeaveCriticalSection(&cs);
}

bool MatchRoom(_ClientInfo* ptr) {
	EnterCriticalSection(&cs);
	for (int i = 0; i < room_count; i++) {
		if (!roominfo[i]->count_full && !roominfo[i]->no_entry) {
			roominfo[i]->client[roominfo[i]->count] = ptr;
			ptr->room = roominfo[i];
	
			if (roominfo[i]->count <= 1) {
				SetEvent(roominfo[i]->hHost);
			}

			roominfo[i]->count++;

			if (roominfo[i]->count >= ROOM_MAX) {
				roominfo[i]->count_full = true;
			}
			LeaveCriticalSection(&cs);
			return true;
		}
	}
	LeaveCriticalSection(&cs);
	return false;
}

bool OutRoom(_ClientInfo* ptr) {
	EnterCriticalSection(&cs);
	for (int i = 0; i < room_count; i++) {
		if (roominfo[i] == ptr->room) {
			for (int j = 0; j < roominfo[i]->count; j++) {
				if (roominfo[i]->client[j] == ptr) {
					roominfo[i]->client[j] = nullptr;

					for (int k = j; k < roominfo[i]->count - 1; k++) {
						roominfo[i]->client[k] = roominfo[i]->client[k + 1];
					}

					roominfo[i]->count--;
					roominfo[i]->count_full = false;

					if (roominfo[i]->count <= 0) {
						LeaveCriticalSection(&cs);
						return false;
					}

					break;
				}
			}
		}
	}
	LeaveCriticalSection(&cs);
	return true;
}


// 서버 통신 관련 함수

// 에러 함수 ( 종료 )
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

// 에러 함수 ( 출력 )
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

// 소켓 초기화 함수
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
	serveraddr.sin_port = htons(SERVER_PORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr,
		sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	return listen_sock;
}

// 사용자 정의 recv 함수
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

// Packing 을 위한 recv 함수
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

// 프로토콜 읽기 함수
int GetProtocol(const char* _ptr)
{
	int protocol;
	memcpy(&protocol, _ptr, sizeof(int));

	return protocol;
}

// Packing
int PackPacket(char* _buf, int _protocol, const char* _str) {
	int size = 0;
	char* ptr = _buf;
	int len = strlen(_str);

	ptr = ptr + sizeof(int);

	memcpy(ptr, &_protocol, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

	memcpy(ptr, &len, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

	memcpy(ptr, _str, len);
	ptr = ptr + len;
	size = size + len;

	ptr = _buf;
	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}

int PackPacket(char* _buf, int _protocol, int _result, const char* _str) {
	int size = 0;
	char* ptr = _buf;
	int len = strlen(_str);

	ptr = ptr + sizeof(int);

	memcpy(ptr, &_protocol, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

	memcpy(ptr, &_result, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

	memcpy(ptr, &len, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

	memcpy(ptr, _str, len);
	ptr = ptr + len;
	size = size + len;

	ptr = _buf;
	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}

// UnPacking
void UnPackPacket(const char* _buf, int& _data) {
	const char* ptr = _buf + sizeof(int);

	memcpy(&_data, ptr, sizeof(int));
	ptr = ptr + sizeof(int);
}