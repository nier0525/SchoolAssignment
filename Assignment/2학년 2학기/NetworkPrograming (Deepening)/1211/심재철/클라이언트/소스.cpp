#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <stdio.h>

#define SERVER_PORT 6470
#define SERVER_IP "127.0.0.1"
#define BUFSIZE 4096
#define MSGSIZE 512

enum PROTOCOL { WAIT = -1, INTRO = 1, ANSWER_INFO, SELECT_NUM, ERROR_NUM, ANSWER_RESULT };
enum RESULT { GIVEUP = -1, WIN = 1, LOSE };

void err_quit(const char *msg);																			// 에러 함수 ( 종료 )
void err_display(const char *msg);																		// 에러 함수 ( 출력 )

SOCKET socket_init();

int recvn(SOCKET s, char *buf, int len, int flags);												// 사용자 지정 Recv 함수
bool PacketRecv(SOCKET _sock, char* _buf);													// Paking 을 위한 Recv 함수
int GetProtocol(const char* _ptr);

int PackPacket(char* _buf, int _protocol, int answer);

void UnPackPacket(const char* _buf, char* _data);
void UnPackPacket(const char* _buf, int& result, char* _data);

int main() {
	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return -1;

	SOCKET sock = socket_init();

	char buf[BUFSIZE];
	int size;

	char msg[MSGSIZE];
	int answer;

	bool flag = false;

	PROTOCOL protocol;
	RESULT result;

	while (1) {
		if (!PacketRecv(sock, buf)) {
			break;
		}

		protocol = (PROTOCOL)GetProtocol(buf);

		switch (protocol) {
		case WAIT:
			ZeroMemory(msg, sizeof(msg));
			UnPackPacket(buf, msg);
			printf("%s\n", msg);
			break;

		case INTRO:
			ZeroMemory(msg, sizeof(msg));
			UnPackPacket(buf, msg);
			printf("%s\n", msg);
			break;

		case ANSWER_INFO:
			ZeroMemory(msg, sizeof(msg));
			UnPackPacket(buf, msg);

			int answer;
			int num;
			num = atoi(msg);

			printf("\n상대가 입력한 수 : %d\n", num);
			printf("입력 : ");
			scanf("%s", msg);

			answer = atoi(msg);
			size = PackPacket(buf, (PROTOCOL)SELECT_NUM, answer);
			retval = send(sock, buf, size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("Select Num Send");
				flag = true;
			}
			break;

		case ERROR_NUM:
			ZeroMemory(msg, sizeof(msg));
			UnPackPacket(buf, msg);
			printf("%s\n", msg);
			break;

		case ANSWER_RESULT:
			ZeroMemory(msg, sizeof(msg));
			UnPackPacket(buf, (int&)result, msg);

			switch (result) {
			case GIVEUP:
			case WIN:
			case LOSE:
				printf("%s\n", msg);
				flag = true;
				break;
			}
			break;
		}
		
		if (flag) {
			break;
		}
	}

	system("pause");
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
int PackPacket(char* _buf, int _protocol, int answer) {
	int size = 0;
	char* ptr = _buf;

	ptr = ptr + sizeof(int);

	memcpy(ptr, &_protocol, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

	memcpy(ptr, &answer, sizeof(int));
	size = size + sizeof(int);

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