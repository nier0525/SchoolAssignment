#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <stdio.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6470
#define BUFSIZE 4096
#define MSGSIZE 512

enum PROTOCOL {
	WAIT = -1,
	INTRO = 1,
	WAIT_QUESTION,
	QUESTION,
	QUESTION_SELECT,
	QUESTION_ERROR,
	GAME_START,
	MY_TRUN,
	OTHER_TRUN,
	VALUE_SELECT,
	VALUE_ERROR,
	VALUE_RESULT,
	GAME_RESULT
};

enum RESULT {
	GIVEUP = -1,
	NO_WIN = 1,
	WIN,
	LOSE
};

// 서버 관련 전역 함수

void err_quit(const char *msg);																			// 에러 함수 ( 종료 )
void err_display(const char *msg);																		// 에러 함수 ( 출력 )

SOCKET socket_init();

int recvn(SOCKET s, char *buf, int len, int flags);
bool PacketRecv(SOCKET _sock, char* _buf);
int GetProtocol(const char* _ptr);

int PackPacket(char* _buf, int _protocol, const char* _str);

void UnPackPacket(const char* _buf, char* _data);
void UnPackPacket(const char* _buf, int& result, char* _data);

int main() {
	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return -1;

	SOCKET sock = socket_init();

	int size;
	int answer[3];
	char msg[MSGSIZE];
	char buf[BUFSIZE];

	bool endflag = false;

	while (1) {
		if (!PacketRecv(sock, buf)) {
			break;
		}

		switch ((PROTOCOL)GetProtocol(buf)) {
		case WAIT:
		case INTRO:
		case WAIT_QUESTION:
		case VALUE_ERROR:
		case GAME_START:
		case VALUE_RESULT:
		case QUESTION:
		case QUESTION_ERROR:
		case MY_TRUN:
			ZeroMemory(msg, sizeof(msg));
			UnPackPacket(buf, msg);
			printf("\n%s\n", msg);
			break;

		case QUESTION_SELECT:
			ZeroMemory(msg, sizeof(msg));
			printf("\n입력 : ");
			scanf("%s", msg);

			size = PackPacket(buf, (PROTOCOL)QUESTION_SELECT, msg);
			retval = send(sock, buf, size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("Question Select Send");
				endflag = true;
			}
			break;

		case OTHER_TRUN:
			ZeroMemory(msg, sizeof(msg));
			printf("\n입력 : ");
			scanf("%s", msg);

			size = PackPacket(buf, (PROTOCOL)VALUE_SELECT, msg);
			retval = send(sock, buf, size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("Value Select Send");
				endflag = true;
			}
			break;

		case GAME_RESULT:
			RESULT result;
			ZeroMemory(msg, sizeof(msg));
			UnPackPacket(buf, (int&)result, msg);
			printf("\n%s\n", msg);

			endflag = true;
			break;
		}

		if (endflag) {
			break;
		}
	}

	closesocket(sock);
	WSACleanup();
	return 0;
}

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

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVER_PORT);
	serveraddr.sin_addr.s_addr = inet_addr(SERVER_IP);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	return sock;
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
int PackPacket(char* _buf, int _protocol, int* answer) {
	int size = 0;
	char* ptr = _buf;

	ptr = ptr + sizeof(int);

	memcpy(ptr, &_protocol, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

	memcpy(ptr, &answer, sizeof(answer));
	size = size + sizeof(answer);

	ptr = _buf;
	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}

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

// UnPacking
void UnPackPacket(const char* _buf, char* _data) {
	const char* ptr = _buf + sizeof(int);
	int len = 0;

	memcpy(&len, ptr, sizeof(int));
	ptr = ptr + sizeof(int);

	memcpy(_data, ptr, len);
	ptr = ptr + len;
}

void UnPackPacket(const char* _buf, int& result, char* _data) {
	const char* ptr = _buf + sizeof(int);
	int len = 0;

	memcpy(&result, ptr, sizeof(int));
	ptr = ptr + sizeof(int);

	memcpy(&len, ptr, sizeof(int));
	ptr = ptr + sizeof(int);

	memcpy(_data, ptr, len);
	ptr = ptr + len;
}