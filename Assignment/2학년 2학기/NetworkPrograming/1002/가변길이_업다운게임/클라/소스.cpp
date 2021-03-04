#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    4092
#define MSGSIZE 512

enum PROTOCOL { INTRO = 1, UP, DOWN, ANSWER_RESULT , WIN };
//enum RESULT { ANSWER_UP, ANSWER_DOWN, ANSWER };

struct _UserInfo {
	int answer;
};

// 소켓 함수 오류 출력 후 종료
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

// 사용자 정의 데이터 수신 함수
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

SOCKET sock_init() {
	int retval;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	return sock;
}

void memcpy_move(int* size, char* ptr, int len) {
	*size += len;
		ptr += len;
}

int main(int argc, char *argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = sock_init();

	// 데이터 통신에 사용할 변수
	char buf[BUFSIZE];
	int len;

	// 서버와 데이터 통신
	while (1) {
		_UserInfo user;
		ZeroMemory(&user, sizeof(user));

		char msg[MSGSIZE];
		int size = 0;
		char *ptr = buf;
		int strsize = 0;
		int usersize = 0;

		PROTOCOL protocol;

		// -------------------------------------------------------------------------------------------

		retval = recvn(sock, (char*)&size, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		retval = recvn(sock, buf, size, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		memcpy(&protocol, ptr, sizeof(PROTOCOL));
		ptr += sizeof(PROTOCOL);

		switch (protocol) {
		case INTRO:
			memcpy(&strsize, ptr, sizeof(int));
			ptr += sizeof(int);

			memcpy(&msg, ptr, strsize);

			msg[strsize] = '\0';
			printf("\n%s", msg);
			break;
		}

		while (1) {
			bool flug = false;

			printf("\n입력 : ");
			scanf("%d", &user.answer);

			size = 0;
			ptr = buf + sizeof(int);
			protocol = ANSWER_RESULT;
			usersize = sizeof(_UserInfo);

			memcpy(ptr, &protocol, sizeof(PROTOCOL));
			size += sizeof(PROTOCOL);
			ptr += sizeof(PROTOCOL);

			memcpy(ptr, &usersize, sizeof(_UserInfo));
			size += sizeof(_UserInfo);
			ptr += sizeof(_UserInfo);

			memcpy(ptr, (char*)&user, usersize);
			size += usersize;
			ptr = buf;

			memcpy(ptr, &size, sizeof(int));

			retval = send(sock, buf, size + sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
			else if (retval == 0) {
				break;
			}

			retval = recvn(sock, (char*)&size, sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0)
				break;

			retval = recvn(sock, buf, size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0)
				break;

			memcpy(&protocol, ptr, sizeof(PROTOCOL));
			ptr += sizeof(PROTOCOL);

			memcpy(&strsize, ptr, sizeof(int));
			ptr += sizeof(int);

			memcpy(&msg, ptr, strsize);

			msg[strsize] = '\0';

			switch (protocol) {
			case UP:
			case DOWN:
				break;

			case WIN:
				flug = true;
				break;
			}

			printf("%s", msg);

			if (flug) {
				break;
			}
		}
		break;
	}

	// closesocket()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}