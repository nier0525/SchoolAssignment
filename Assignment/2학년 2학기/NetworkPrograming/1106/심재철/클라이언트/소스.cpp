#pragma comment (lib, "ws2_32.lib")
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 4096
#define MSGSIZE 512

enum GAME_RESULT { WIN, LOSE, DRAW };
enum PROTOCOL { WAIT, INTRO, INFO, RESULT };

int recvn(SOCKET sock, char* buf, int len, int flug);
void err_quit(const char* msg);
void err_display(const char* msg);

SOCKET SocketInit();

int memcpy_send(void* ptr, const void* dst, size_t size, int* len);
int memcpy_recv(void* dst, const void* ptr, size_t size);

bool Packing_Recv(SOCKET sock, char* buf);
int GetProtocol(char* buf);

int Pakcing(char* buf, const int protocol);
int Pakcing(char* buf, const int protocol, const char* msg);

int UnPaking(const char* buf, char* msg);
int UnPaking(const char* buf, int* result, char* msg);

int main() {
	WSADATA wsa;
	if (WSAStartup(MAKELONG(2, 2), &wsa) != 0) {
		err_quit("WSAStartup");
	}

	SOCKET sock = SocketInit();

	char buf[BUFSIZE];
	char msg[MSGSIZE];

	bool flug = false;

	int len = 0;

	GAME_RESULT result = DRAW;

	while (!flug) {
		int retval = 0;
		int size = 0;

		if (!Packing_Recv(sock, buf)) {
			break;
		}

		PROTOCOL protocol = (PROTOCOL)GetProtocol(buf);

		switch (protocol) {
		case WAIT:
			len = UnPaking(buf, msg);
			msg[len] = '\0';
			printf("%s\n\n", msg);

			size = Pakcing(buf, (int)WAIT);

			retval = send(sock, buf, size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send");
				flug = true;
			}
			continue;

		case INTRO:
			len = UnPaking(buf, msg);
			msg[len] = '\0';
			printf("%s\n", msg);

			while (1) {
				printf("입력 : ");
				scanf("%s", msg);

				msg[1] = '\0';

				if (atoi(msg) >= 3) {
					printf("\n잘못된 입력입니다. 다시 입력하세요.\n");
					continue;
				}
				break;
			}

			size = Pakcing(buf, (int)INFO, msg);
			
			retval = send(sock, buf, size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send");
				flug = true;
			}
			continue;

		case RESULT:
			len = UnPaking(buf, (int*)result, msg);
			msg[len] = '\0';
			printf("%s\n", msg);
			break;
		}
		break;
	}

	system("pause");
	closesocket(sock);
	WSACleanup();
	return 0;
}

int recvn(SOCKET sock, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0)
	{
		received = recv(sock, ptr, left, flags);
		if (received == SOCKET_ERROR)
		{
			return SOCKET_ERROR;
		}
		else if (received == 0)
		{
			break;
		}

		left -= received;
		ptr += received;
	}

	return (len - left);
}

void err_quit(const char* msg)
{
	LPVOID lpmsgbuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&lpmsgbuf, 0, NULL);

	MessageBox(NULL, (LPCSTR)lpmsgbuf,
		msg, MB_ICONERROR);
	LocalFree(lpmsgbuf);
	exit(-1);
}

void err_display(const char* msg)
{
	LPVOID lpmsgbuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&lpmsgbuf, 0, NULL);

	printf("[%s]%s\n", msg, (LPCSTR)lpmsgbuf);
	LocalFree(lpmsgbuf);
}

SOCKET SocketInit() {
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		err_quit("socket()");
	}

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVERPORT);
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);

	int retval = connect(sock, (SOCKADDR*)&serveraddr,
		sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		err_quit("connect()");
	}

	return sock;
}

// 가변 길이 패킹 함수
int memcpy_send(void* ptr, const void* dst, size_t size, int* len) {
	memcpy(ptr, dst, size);	// 메모리 복사
	*len += size;						// 메모리의 크기를 인자값으로 받은 크기만큼 증가
	return size;						// 인자값으로 받은 크기 리턴
}

// 가변 길이 언패킹 함수
int memcpy_recv(void* dst, const void* ptr, size_t size) {
	memcpy(dst, ptr, size);	// 메모리 복사
	return size;						// 인자값으로 받은 크기 리턴
}

bool Packing_Recv(SOCKET s, char* buf) {
	int retval;
	int size;

	retval = recvn(s, (char*)&size, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recvn");
		return false;
	}
	else if (retval == 0) {
		return false;
	}

	// 버퍼 수신
	retval = recvn(s, buf, size, 0);
	if (retval == SOCKET_ERROR) {
		err_display("recvn");
		return false;
	}
	else if (retval == 0) {
		return false;
	}

	return true;
}

int GetProtocol(char* buf) {
	int protocol;
	memcpy(&protocol, buf, sizeof(int));
	return protocol;
}

int Pakcing(char* buf, const int protocol) {
	int size = 0;
	char* ptr = buf + sizeof(int);

	ptr += memcpy_send(ptr, &protocol, sizeof(int), &size);

	ptr = buf;
	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}

int Pakcing(char* buf, const int protocol, const char* msg) {
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

int UnPaking(const char* buf, char* msg) {
	const char* ptr = buf + sizeof(int);
	int len = 0;

	ptr += memcpy_recv(&len, ptr, sizeof(int));
	ptr += memcpy_recv(msg, ptr, len);

	return len;
}

int UnPaking(const char* buf, int* result, char* msg) {
	const char* ptr = buf + sizeof(int);
	int len = 0;

	ptr += memcpy_recv(&result, ptr, sizeof(int));
	ptr += memcpy_recv(&len, ptr, sizeof(int));
	ptr += memcpy_recv(msg, ptr, len);

	return len;
}
