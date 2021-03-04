#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    4092

enum PROTOCOL
{
	INTRO = 1,
	INTRO_SELECT_MENU,
	LOGIN_SELECT_MENU,
	ID_PW_MSG,
	NEW_INFO,
	LOGIN_INFO,
	NEW_RESULT,
	LOGIN_RESULT,
	LOGIN_INTRO,
	LOGOUT,
	REMOVE
};

enum RESULT
{
	NODATA = -1,
	ID_EXIST = 1,
	ID_ERROR,
	PW_ERROR,
	LOGIN_ERROR,
	NEW_SUCCESS,
	LOGIN_SUCCESS,
};

enum INTRO_MENU
{
	NEW_MENU = 1,
	LOGIN_MENU,
	EXIT_MENU
};

struct User_Info {
	char id[128];
	char pw[128];

	bool state;
};

// ���� �Լ� ���� ��� �� ����
void err_quit(const char* msg)
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

// ���� �Լ� ���� ���
void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// ����� ���� ������ ���� �Լ�
int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
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

bool UnPackingRecv(SOCKET s, char* buf) {
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

	// ���� ����
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

// ���� ���� ��ŷ �Լ�
int memcpy_send(void* ptr, const void* dst, size_t size, int* len) {
	memcpy(ptr, dst, size);	// �޸� ����
	*len += size;						// �޸��� ũ�⸦ ���ڰ����� ���� ũ�⸸ŭ ����
	return size;						// ���ڰ����� ���� ũ�� ����
}

// ���� ���� ����ŷ �Լ�
int memcpy_recv(void* dst, const void* ptr, size_t size) {
	memcpy(dst, ptr, size);	// �޸� ����
	return size;						// ���ڰ����� ���� ũ�� ����
}

PROTOCOL GetProtocol(char* buf) {
	PROTOCOL protocol;
	memcpy(&protocol, buf, sizeof(PROTOCOL));

	return protocol;
}

// Packing
int Packing(char* buf, PROTOCOL protocol) {
	int size = 0;
	char* ptr = buf + sizeof(int);

	ptr += memcpy_send(ptr, &protocol, sizeof(PROTOCOL), &size);

	ptr = buf;
	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}

int Packing(char* buf, PROTOCOL protocol, const char* str1) {
	int size = 0;
	char* ptr = buf + sizeof(int);
	int len = strlen(str1);

	ptr += memcpy_send(ptr, &protocol, sizeof(PROTOCOL), &size);

	ptr += memcpy_send(ptr, &len, sizeof(int), &size);
	ptr += memcpy_send(ptr, str1, len, &size);

	ptr = buf;
	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}

int Packing(char* buf, PROTOCOL protocol, const char* str1, const char* str2) {
	int size = 0;
	char* ptr = buf + sizeof(int);
	int len = strlen(str1);
	int len2 = strlen(str2);

	ptr += memcpy_send(ptr, &protocol, sizeof(PROTOCOL), &size);

	ptr += memcpy_send(ptr, &len, sizeof(int), &size);
	ptr += memcpy_send(ptr, str1, len, &size);
	ptr += memcpy_send(ptr, &len2, sizeof(int), &size);
	ptr += memcpy_send(ptr, str2, len2, &size);

	ptr = buf;
	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}

// UnPacking
void UnPacking(const char* buf, char* str) {
	const char* ptr = buf + sizeof(PROTOCOL);
	int len = 0;

	ptr += memcpy_recv(&len, ptr, sizeof(int));
	ptr += memcpy_recv(str, ptr, len);
}

void UnPacking(const char* buf, RESULT& result, char* str) {
	const char* ptr = buf + sizeof(PROTOCOL);
	int len = 0;

	ptr += memcpy_recv(&result, ptr, sizeof(RESULT));
	ptr += memcpy_recv(&len, ptr, sizeof(int));
	ptr += memcpy_recv(str, ptr, len);
}

void UnPacking(const char* buf, char* str, char* str2) {
	const char* ptr = buf + sizeof(PROTOCOL);
	int len = 0;
	int len2 = 0;

	ptr += memcpy_recv(&len, ptr, sizeof(int));
	ptr += memcpy_recv(str, ptr, len);
	ptr += memcpy_recv(&len2, ptr, sizeof(int));
	ptr += memcpy_recv(str2, ptr, len2);
}

int main(int argc, char* argv[])
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	// ������ ��ſ� ����� ����
	char buf[BUFSIZE + 1];
	int len;

	char msg[128];
	int size = 0;

	User_Info user;
	PROTOCOL protocol;
	RESULT result;

	// ������ ������ ���
	while (1) {
		if (!UnPackingRecv(sock, buf))
		{
			break;
		}

		protocol = GetProtocol(buf);

		switch (protocol) {
		case PROTOCOL::INTRO:
		case PROTOCOL::LOGIN_INTRO:
			ZeroMemory(msg, sizeof(msg));
			UnPacking(buf, msg);

			printf("%s", msg);
			ZeroMemory(msg, sizeof(msg));
			scanf("%s", msg);

			switch (protocol)
			{
			case PROTOCOL::INTRO:
				size = Packing(buf, PROTOCOL::INTRO_SELECT_MENU, msg);
				break;

			case PROTOCOL::LOGIN_INTRO:
				size = Packing(buf, PROTOCOL::LOGIN_SELECT_MENU, msg);
				break;
			}
			retval = send(sock, buf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("send");
			}
			break;

		case PROTOCOL::ID_PW_MSG:
			ZeroMemory(&user, sizeof(User_Info));
			UnPacking(buf, user.id, user.pw);

			printf("%s", user.id);
			scanf("%s", user.id);
			printf("%s", user.pw);
			scanf("%s", user.pw);

			switch (atoi(msg)) 
			{
			case NEW_MENU:
				size = Packing(buf, PROTOCOL::NEW_INFO, user.id, user.pw);
				break;

			case LOGIN_MENU:
				size = Packing(buf, PROTOCOL::LOGIN_INFO, user.id, user.pw);
				break;
			}
			
			retval = send(sock, buf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("send");
			}
			break;

		case PROTOCOL::NEW_RESULT:
		case PROTOCOL::LOGIN_RESULT:
			ZeroMemory(msg, sizeof(msg));
			UnPacking(buf, result, msg);

			printf("%s", msg);
			break;

		case PROTOCOL::LOGOUT:
		case PROTOCOL::REMOVE:
			ZeroMemory(msg, sizeof(msg));
			UnPacking(buf, msg);

			printf("%s", msg);
			break;
		}
	}

	// closesocket()
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}