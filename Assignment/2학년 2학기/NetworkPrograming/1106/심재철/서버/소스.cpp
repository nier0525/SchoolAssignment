#pragma comment (lib, "ws2_32")
#include <WinSock2.h>

#include <stdio.h>
#include <stdlib.h>

#define SERVERPORT 9000
#define BUFSIZE 4096
#define MSGSIZE 512

#define CLIENT_SIZE 40
#define ROOM_SIZE 2

#define WAIT_MSG "상대를 기다리고 있습니다. . ."
#define INTRO_MSG "상대가 입장했습니다. 가위(0) , 바위 (1), 보(2) 중 하나를 입력하세요."

#define WINNER_MSG "승리하였습니다."
#define LOSER_MSG "패배하였습니다."
#define DRAW_MSG "무승부 입니다."
#define ERROR_MSG "상대방이 도중이 나갔기 때문에 부전승 입니다."

enum GAME_RESULT { WIN, LOSE, DRAW, WIN_ERROR };
enum PROTOCOL { WAIT, INTRO, INFO, RESULT };

struct User {
	char msg[MSGSIZE];
};

struct Client_Info {
	SOCKET sock;
	SOCKADDR_IN addr;
	char buf[BUFSIZE];

	User user;
};

struct Room_Info {
	Client_Info* p[ROOM_SIZE];
};

Client_Info* client_info[CLIENT_SIZE];
int client_count = 0;

CRITICAL_SECTION cs;

Client_Info* Add_ClientInfo(SOCKET sock, SOCKADDR_IN addr);
bool Remove_ClientInfo(Client_Info* ptr);

int recvn(SOCKET sock, char* buf, int len, int flug);
void err_quit(const char* msg);
void err_display(const char* msg);

SOCKET SocketInit();

int memcpy_send(void* ptr, const void* dst, size_t size, int* len);
int memcpy_recv(void* dst, const void* ptr, size_t size);

bool Packing_Recv(SOCKET sock, char* buf);
int GetProtocol(char* buf);

int Packing(char* buf, const int protocol, const char* msg);
int Packing(char* buf, const int protocol , const int result, const char* msg);

int UnPaking(const char* buf, char* msg);

DWORD WINAPI GameSystem(LPVOID arg);

int main() {
	WSADATA wsa;
	if (WSAStartup(MAKELONG(2, 2), &wsa) != 0) {
		err_quit("WSAStartup");
	}

	SOCKET listen_sock = SocketInit();

	SOCKET client_sock;
	SOCKADDR_IN client_addr;
	int addrlen;

	InitializeCriticalSection(&cs);

	while (1) {
		addrlen = sizeof(SOCKADDR_IN);
		client_sock = accept(listen_sock, (SOCKADDR*)&client_addr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept");
			continue;
		}

		if (client_count != 0 && client_count % 2 == 1) {
			if (!Packing_Recv(client_info[client_count - 1]->sock, client_info[client_count - 1]->buf)) {
				Remove_ClientInfo(client_info[client_count - 1]);
			}
		}

		Client_Info* temp = Add_ClientInfo(client_sock, client_addr);

		if (client_count % 2 == 0) {
			Room_Info* ptr = new Room_Info;
			ptr->p[0] = client_info[client_count - 2];
			ptr->p[1] = client_info[client_count - 1];

			HANDLE hThread = CreateThread(nullptr, 0, GameSystem, (LPVOID)ptr, 0, nullptr);

			if (hThread == nullptr) {
				for (int i = client_count; i >= client_count - 2; i--) {
					Remove_ClientInfo(ptr->p[i]);
				}
			}
			else {
				CloseHandle(hThread);
			}
		}
		
		else {
			int size = Packing(temp->buf, (int)WAIT, WAIT_MSG);
			int retval = send(temp->sock, temp->buf, size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send");
				break;
			}
		}
	}

	DeleteCriticalSection(&cs);
	WSACleanup();
	return 0;
}


// 접속 클라이언트 정보 추가
Client_Info* Add_ClientInfo(SOCKET sock, SOCKADDR_IN addr) {
	EnterCriticalSection(&cs);

	Client_Info* ptr = new Client_Info;
	ZeroMemory(ptr, sizeof(Client_Info));

	ptr->sock = sock;
	memcpy(&ptr->addr, &addr, sizeof(SOCKADDR_IN));

	printf("[ 클라이언트 접속 ] IP : %s , PORT : %d\n", inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));

	client_info[client_count++] = ptr;

	LeaveCriticalSection(&cs);
	return ptr;
}

// 접속 클라이언트 정보 삭제
bool Remove_ClientInfo(Client_Info* ptr) {
	printf("[ 클라이언트 종료 ] IP : %s , PORT : %d\n", inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));
	closesocket(ptr->sock);

	EnterCriticalSection(&cs);

	for (int i = 0; i < client_count; i++) {
		if (client_info[i] == ptr) {
			delete client_info[i];
			for (int j = i; j < client_count - 1; j++) {
				client_info[j] = client_info[j + 1];
			}
			ZeroMemory(client_info[client_count - 1], sizeof(Client_Info));
			client_count--;
			LeaveCriticalSection(&cs);
			return true;
		}
	}

	LeaveCriticalSection(&cs);
	return false;
}

// 사용자 지정 Recv 함수
int recvn(SOCKET sock, char* buf, int len, int flug) {
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(sock, ptr, left, flug);

		if (received == SOCKET_ERROR) {
			return SOCKET_ERROR;
		}
		else if (received == 0) {
			break;
		}

		left -= received;
		ptr += received;
	}

	return (len - left);
}

// 에러 체크 ( 종료 )
void err_quit(const char* msg) {
	LPVOID lpmsgbuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&lpmsgbuf, 0, NULL);

	MessageBox(nullptr, (LPCTSTR)lpmsgbuf, msg, MB_ICONERROR);
	LocalFree(lpmsgbuf);
	exit(1);
}

// 에러 체크 ( 출력 )
void err_display(const char* msg) {
	LPVOID lpmsgbuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&lpmsgbuf, 0, NULL);

	printf("[%s] %s\n", msg, (LPCSTR)lpmsgbuf);
	LocalFree(lpmsgbuf);
}

// 소켓 초기화 함수
SOCKET SocketInit() {
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		err_quit("socket");
	}

	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(SOCKADDR_IN));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(SERVERPORT);

	int retval = bind(sock, (SOCKADDR*)&addr, sizeof(addr));
	if (retval == SOCKET_ERROR) {
		err_quit("bind");
	}

	retval = listen(sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) {
		err_quit("listen");
	}

	return sock;
}

// 패킹 메모리 복사 함수
int memcpy_send(void* ptr, const void* dst, size_t size, int* len) {
	memcpy(ptr, dst, size);
	*len += size;
	return size;
}

// 언패킹 메모리 복사 함수
int memcpy_recv(void* dst, const void* ptr, size_t size) {
	memcpy(dst, ptr, size);
	return size;
}

// 패킹 Recv 함수
bool Packing_Recv(SOCKET sock, char* buf) {
	int retval;
	int size;

	retval = recvn(sock, (char*)&size, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recvn");
		return false;
	}
	else if (retval == 0) {
		return false;
	}

	retval = recvn(sock, buf, size, 0);
	if (retval == SOCKET_ERROR) {
		err_display("recvn");
		return false;
	}
	else if (retval == 0) {
		return false;
	}

	return true;
}

// 프로토콜 호출 함수
int GetProtocol(char* buf) {
	int protocol;
	memcpy(&protocol, buf, sizeof(int));
	return protocol;
}

int Packing(char* buf, const int protocol, const char* msg) {
	int size = 0;
	char* ptr = buf + sizeof(int);
	int len = strlen(msg);

	ptr += memcpy_send(ptr, &protocol, sizeof(int), &size);
	ptr += memcpy_send(ptr, &len, sizeof(int), &size);
	ptr += memcpy_send(ptr, msg, len, &size);

	ptr = buf;

	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}

int Packing(char* buf, const int protocol, const int result, const char* msg) {
	int size = 0;
	char* ptr = buf + sizeof(int);
	int len = strlen(msg);

	ptr += memcpy_send(ptr, &protocol, sizeof(int), &size);
	ptr += memcpy_send(ptr, &result, sizeof(int), &size);
	ptr += memcpy_send(ptr, &len, sizeof(int), &size);
	ptr += memcpy_send(ptr, msg, len, &size);

	ptr = buf;

	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}

int UnPaking(const char* buf, char* msg) {
	const char* ptr = buf + sizeof(int);
	int len = 0;

	ptr += memcpy_recv(&len, ptr, sizeof(int));
	ptr += memcpy_recv(msg, ptr, len);

	return len;
}

DWORD WINAPI GameSystem(LPVOID arg) {
	Room_Info* ptr = (Room_Info*)arg;

	bool flug = false;
	bool error = false;
	int retval;
	int size;

	for (int i = 0; i < ROOM_SIZE; i++) {
		size = Packing(ptr->p[i]->buf, (int)INTRO, INTRO_MSG);
		retval = send(ptr->p[i]->sock, ptr->p[i]->buf, size, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send");
			flug = true;
			break;
		}
	}

	while (!flug) {
		for (int i = 0; i < ROOM_SIZE; i++) {
			if (!Packing_Recv(ptr->p[i]->sock, ptr->p[i]->buf)) {
				error = true;
				flug = true;
			}
		}

		PROTOCOL protocol = (PROTOCOL)GetProtocol(ptr->p[1]->buf);

		switch (protocol) {
		case INFO:
			int p1, p2;

			for (int i = 0; i < ROOM_SIZE; i++) {
				UnPaking(ptr->p[i]->buf, ptr->p[i]->user.msg);
			}

			p1 = atoi(ptr->p[0]->user.msg);
			p2 = atoi(ptr->p[1]->user.msg);

			if (p1 == p2) {
				size = Packing(ptr->p[0]->buf, (int)RESULT, (int)DRAW, DRAW_MSG);
				size = Packing(ptr->p[1]->buf, (int)RESULT, (int)DRAW, DRAW_MSG);
			}
			else if ((p1 + p2) % 2 == 0) {
				if (p1 < p2) {
					size = Packing(ptr->p[0]->buf, (int)RESULT, (int)WIN, WINNER_MSG);
					size = Packing(ptr->p[1]->buf, (int)RESULT, (int)LOSE, LOSER_MSG);
				}
				else {
					size = Packing(ptr->p[0]->buf, (int)RESULT, (int)LOSE, LOSER_MSG);
					size = Packing(ptr->p[1]->buf, (int)RESULT, (int)WIN, WINNER_MSG);
				}
			}
			else if ((p1 + p2) % 2 == 1) {
				if (p1 < p2) {
					size = Packing(ptr->p[0]->buf, (int)RESULT, (int)LOSE, LOSER_MSG);
					size = Packing(ptr->p[1]->buf, (int)RESULT, (int)WIN, WINNER_MSG);
				}
				else {
					size = Packing(ptr->p[0]->buf, (int)RESULT, (int)WIN, WINNER_MSG);
					size = Packing(ptr->p[1]->buf, (int)RESULT, (int)LOSE, LOSER_MSG);
				}
			}

			if (error) {
				size = Packing(ptr->p[1]->buf, (int)RESULT, (int)WIN_ERROR, ERROR_MSG);

				retval = send(ptr->p[1]->sock, ptr->p[1]->buf, size, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send");
					flug = true;
					break;
				}
				break;
			}
			else {
				for (int i = 0; i < ROOM_SIZE; i++) {
					retval = send(ptr->p[i]->sock, ptr->p[i]->buf, size, 0);
					if (retval == SOCKET_ERROR) {
						err_display("send");
						flug = true;
						break;
					}
				}
			}
			break;

		default:
			size = Packing(ptr->p[0]->buf, (int)RESULT, (int)WIN_ERROR, ERROR_MSG);

			retval = send(ptr->p[0]->sock, ptr->p[0]->buf, size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send");
				flug = true;
				break;
			}
			break;
		}
		break;
	}

	for (int i = ROOM_SIZE - 1; i >= 0; i--) {
		Remove_ClientInfo(ptr->p[i]);
	}
	return 0;
}