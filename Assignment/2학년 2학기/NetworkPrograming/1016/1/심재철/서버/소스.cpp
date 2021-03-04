#pragma comment (lib, "ws2_32.lib")
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#define SERVERPORT 9000
#define BUFSIZE 4096

#define INTRO_MSG "ID와 PW를 입력하세요!\n"
#define ID_ERROR_MSG "없는 ID입니다.\n"
#define PW_ERROR_MSG "PW가 틀렸습니다.\n"
#define LOGIN_MSG "로그인에 성공했습니다.\n"

enum RESULT
{ 
	ID_ERROR=1, 
	PW_ERROR, 
	LOGIN
};

struct _LoginInfo
{
	char ID[255];
	char PW[255];
}LoginInfo[3] = { {"aaa","111"},
				  {"bbb","222"},
				  {"kja0204", "1234"}};

enum PROTOCOL
{
	INTRO=1,
	LOGIN_INFO,
	LOGIN_RESULT
};


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

int Packing(char* buf, PROTOCOL protocol, const char* data) {
	int size = 0;
	char* ptr = buf + sizeof(int);
	int len = strlen(data);

	ptr += memcpy_send(ptr, &protocol, sizeof(PROTOCOL), &size);
	ptr += memcpy_send(ptr, &len, sizeof(int), &size);
	ptr += memcpy_send(ptr, data, len, &size);

	ptr = buf;

	// 버퍼의 크기 패킹
	memcpy(ptr, &size, sizeof(int));

	return size;
}

int Packing(char* buf, PROTOCOL protocol, int result, const char* data) {
	int size = 0;
	char* ptr = buf + sizeof(int);
	int len = strlen(data);

	ptr += memcpy_send(ptr, &protocol, sizeof(PROTOCOL), &size);
	ptr += memcpy_send(ptr, &result, sizeof(int), &size);
	ptr += memcpy_send(ptr, &len, sizeof(int), &size);
	ptr += memcpy_send(ptr, data, len, &size);

	ptr = buf;

	// 버퍼의 크기 패킹
	memcpy(ptr, &size, sizeof(int));

	return size;
}

void UnPacking(const char* buf, char* str1, char* str2) {

	const char* ptr = buf + sizeof(PROTOCOL);
	
	int len = strlen(str1);
	int len2 = strlen(str2);

	ptr += memcpy_recv(&len, ptr, sizeof(int));
	ptr += memcpy_recv(str1, ptr, len);

	ptr += memcpy_recv(&len2, ptr, sizeof(int));
	ptr += memcpy_recv(str2, ptr, len2);
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


int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa)
		!=0)
	{
		err_quit("WSAStartUP()");
	}

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
	{
		err_quit("socket()");
	}

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVERPORT);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int retval = bind(listen_sock,
		(SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		err_quit("bind()");
	}

	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		err_quit("listen()");
	}

	SOCKADDR_IN clientaddr;
	int addrlen;
	SOCKET client_sock;
	char buf[BUFSIZE];

	while (1)
	{
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock,
			(SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			continue;//break;
		}

		printf("클라이언트 접속:%s :%d\n",
			inet_ntoa(clientaddr.sin_addr),
			ntohs(clientaddr.sin_port));

		while (1)
		{
			int size = 0;

			size = Packing(buf, INTRO, INTRO_MSG);

			retval = send(client_sock, buf, size+sizeof(int),0);
			if (retval == SOCKET_ERROR)
			{
				err_display("send()");
				break;
			}			

			if (!packing_recvn(client_sock, buf)) {
				break;
			}

			PROTOCOL protocol = GetProtocol(buf);

			_LoginInfo logininfo;

			switch (protocol)
			{
			case LOGIN_INFO:
				ZeroMemory(&logininfo, sizeof(_LoginInfo));
				UnPacking(buf, logininfo.ID, logininfo.PW);

				logininfo.ID[strlen(logininfo.ID)] = '\0';
				logininfo.PW[strlen(logininfo.PW)] = '\0';

				break;
			}

			bool flag = false;		
			RESULT result;
			char msg[BUFSIZE];

			for (int i = 0; i < 3; i++)
			{
				if (!strcmp(LoginInfo[i].ID, logininfo.ID))
				{
					flag = true;
					if (!strcmp(LoginInfo[i].PW, logininfo.PW))
					{
						result = LOGIN;
						strcpy(msg, LOGIN_MSG);
					}
					else
					{
						result = PW_ERROR;
						strcpy(msg, PW_ERROR_MSG);
					}
				}
			}

			if (!flag)
			{
				result = ID_ERROR;
				strcpy(msg, ID_ERROR_MSG);
			}

			size = Packing(buf, LOGIN_RESULT, result, msg);

			retval = send(client_sock, buf, size+sizeof(int),0);
			if (retval == SOCKET_ERROR)
			{
				err_display("send()");
				break;
			}

			if (result == LOGIN)
			{
				break;
			}
		}

		closesocket(client_sock);

		printf("클라이언트 종료:%s :%d\n",
			inet_ntoa(clientaddr.sin_addr),
			ntohs(clientaddr.sin_port));
	}

	closesocket(listen_sock);

	WSACleanup();
	return 0;
}