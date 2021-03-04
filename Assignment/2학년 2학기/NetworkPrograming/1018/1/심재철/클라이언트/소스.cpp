#pragma comment (lib, "ws2_32.lib")
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 4096

enum RESULT
{
	ID_ERROR = 1,
	PW_ERROR,
	LOGIN
};

enum PROTOCOL
{
	INTRO = 1,
	LOGIN_INFO,
	LOGIN_RESULT
};

struct _LoginInfo
{
	char ID[255];
	char PW[255];
};


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

PROTOCOL GetProtocol(char* buf) {
	PROTOCOL protocol;
	memcpy(&protocol, buf, sizeof(PROTOCOL));

	return protocol;
}

bool packing_recvn(SOCKET s, char* buf) {
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

	return size;
}

int UnPacking(const char* buf, char* str) {
	const char* ptr = buf + sizeof(PROTOCOL);
	int len = strlen(str);

	ptr += memcpy_recv(&len, ptr, sizeof(int));
	ptr += memcpy_recv(str, ptr, len);

	return len;
}
 
int UnPacking(const char* buf, int data, char* str) {
	const char* ptr = buf + sizeof(PROTOCOL);
	int len = strlen(str);

	ptr += memcpy_recv(&data, ptr, sizeof(int));
	ptr += memcpy_recv(&len, ptr, sizeof(int));
	ptr += memcpy_recv(str, ptr, len);

	return len;
}

int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa)
		!= 0)
	{
		err_quit("WSAStartUP()");
	}

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

	char buf[BUFSIZE];
	char msg[BUFSIZE];

	while (1)
	{
		int size = 0;
		int retval = 0;
		int strsize = 0;

		bool flag = false;

		if (!packing_recvn(sock, buf)) {
			break;
		}

		PROTOCOL protocol = GetProtocol(buf);

		switch (protocol)
		{
		case INTRO:
			strsize = UnPacking(buf, msg);

			msg[strsize] = '\0';
			printf("%s\n", msg);
			break;

		case LOGIN_RESULT:
			RESULT result;
			strsize = UnPacking(buf, result, msg);

			msg[strsize] = '\0';
			printf("%s\n", msg);

			if (result == LOGIN)
			{
				flag = true;
			}
			else
			{
				continue;
			}

			break;

		}	

		if (flag)
		{
			break;
		}
	
		_LoginInfo logininfo;
		ZeroMemory(&logininfo, sizeof(_LoginInfo));
		
		printf("ID:");
		scanf("%s", logininfo.ID);
		printf("PW:");
		scanf("%s", logininfo.PW);

		protocol = LOGIN_INFO;
		size = Packing(buf, protocol, logininfo.ID, logininfo.PW);

		retval = send(sock, buf, size + sizeof(int), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}
	}

	system("pause");
	closesocket(sock);

	WSACleanup();
	return 0;
}