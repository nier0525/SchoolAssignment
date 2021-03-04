#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    4092
#define MSGSIZE 512

enum PROTOCOL { INTRO = 1, UP, DOWN, ANSWER_RESULT, WIN };
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

int Memcpy_Send(void* ptr, const void* data, size_t size, int* len) {
	memcpy(ptr, data, size);
	*len += size;
	return size;
}

int Memcpy_Recv(void* data, const void* ptr, size_t size) {
	memcpy(data, ptr, size);
	return size;
}

bool Packing_Recvn(SOCKET client_sock, char* buf) {
	int retval = 0;
	int size = 0;

	retval = recvn(client_sock, (char*)&size, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		return false;
	}
	else if (retval == 0)
		return false;

	retval = recvn(client_sock, buf, size, 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		return false;
	}
	else if (retval == 0)
		return false;

	return true;
}

int GetProtocol(char* buf) {
	int protocol;
	memcpy(&protocol, buf, sizeof(int));
	return protocol;
}

int Packing(char* buf, const int* protocol, const char* data) {
	char* ptr = buf + sizeof(int);
	int size = 0;
	int len = sizeof(data);

	ptr += Memcpy_Send(ptr, &protocol, sizeof(int), &size);
	ptr += Memcpy_Send(ptr, &len, sizeof(int), &size);
	ptr += Memcpy_Send(ptr, data, len, &size);

	ptr = buf;

	memcpy(ptr, &size, sizeof(int));

	return size;
}

int UnPacking(const char* buf, char* data) {
	const char* ptr = buf + sizeof(int);
	int len = 0;

	ptr += Memcpy_Recv(&len, ptr, sizeof(int));
	ptr += Memcpy_Recv(data, ptr, len);

	return len;
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
		char msg[MSGSIZE];
		int size = 0;
		int strsize = 0;

		// -------------------------------------------------------------------------------------------

		if (!Packing_Recvn(sock, buf)) {
			break;
		}

		PROTOCOL protocol = (PROTOCOL)GetProtocol(buf);

		switch (protocol) {
		case INTRO:
			strsize = UnPacking(buf, msg);

			msg[strsize] = '\0';
			printf("\n%s", msg);
			break;
		}

		while (1) {
			bool flug = false;

			_UserInfo user;
			ZeroMemory(&user, sizeof(user));

			printf("\n입력 : ");
			scanf("%d", &user.answer);

			size = Packing(buf, (int*)ANSWER_RESULT, (char*)&user);

			retval = send(sock, buf, size + sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			if (!Packing_Recvn(sock, buf)) {
				return false;
			}

			protocol = (PROTOCOL)GetProtocol(buf);

			switch (protocol) {
			case UP:
			case DOWN:
				break;

			case WIN:
				flug = true;
				break;
			}

			strsize = UnPacking(buf, msg);
			msg[strsize] = '\0';

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