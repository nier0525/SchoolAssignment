#pragma comment (lib,"ws2_32")
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#define SERVERPORT 9000
#define BUFSIZE 512
#define MAX 100

#define ID "ID : "
#define PASS "PASS : "

typedef struct User {
	char id[10];
	char pass[10];
};

void Err_Quit(const char*);
void Err_DisPlay(const char*);

SOCKET SockInit() {
	int retval = 0;

	SOCKET Listen_Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (Listen_Sock == INVALID_SOCKET) {
		Err_Quit("socket()");
	}

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);

	retval = bind(Listen_Sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	if (retval == SOCKET_ERROR) {
		Err_Quit("Bind()");
	}

	retval = listen(Listen_Sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) {
		Err_Quit("Listen()");
	}

	return Listen_Sock;
}

void Err_Quit(const char* msg) {
	LPVOID IpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&IpMsgBuf, 0, NULL);

	MessageBox(NULL, (LPCSTR)IpMsgBuf, msg, MB_ICONERROR);
	LocalFree(IpMsgBuf);
	ExitProcess(0);
}

void Err_DisPlay(const char* msg) {
	LPVOID IpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&IpMsgBuf, 0, NULL);

	printf("[%s] %s\n", msg, (char*)IpMsgBuf);
	LocalFree(IpMsgBuf);
}

int recvn(SOCKET s, char* buf, int len, int flug) {
	int retval;

	char* ptr = buf;
	int left = len;

	while (left > 0) {
		retval = recv(s, ptr, len, flug);
		if (retval == SOCKET_ERROR) {
			Err_DisPlay("Recv()");
			break;
		}
		else if (retval == 0) {
			break;
		}

		left -= retval;
		ptr += retval;
	}

	return (len - left);
}

int main() {
	int retval;
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		Err_Quit("WSAStartup()");
	}

	SOCKET Listen_Sock = SockInit();

	SOCKET Client;
	SOCKADDR_IN ClientServer;
	int len;
	int userlen;

	char success[BUFSIZE];
	char buf[BUFSIZE];

	User user[MAX];
	ZeroMemory(&user, sizeof(user));

	strcpy(user[0].id, "shim");
	strcpy(user[0].pass, "123");
	strcpy(user[1].id, "Lee");
	strcpy(user[1].pass, "456");

	while (1) {
		len = sizeof(ClientServer);
		Client = accept(Listen_Sock, (SOCKADDR*)&ClientServer, &len);
		if (Client == INVALID_SOCKET) {
			Err_DisPlay("Accept()");
		}

		printf("[TCP/IP 클라이언트 접속] IP : %s , PORT : %d\n", inet_ntoa(ClientServer.sin_addr),
			ntohs(ClientServer.sin_port));

		bool flug = false;

		while (1) {

			retval = recvn(Client, (char*)&userlen, sizeof(int), 0);

			if (retval == SOCKET_ERROR) {
				Err_DisPlay("Recv()");
				break;
			}
			
			retval = send(Client, (char*)ID, sizeof(ID), 0);
			if (retval == SOCKET_ERROR) {
				Err_DisPlay("Send()");
				break;
			}

			retval = send(Client, (char*)PASS, sizeof(PASS), 0);
			if (retval == SOCKET_ERROR) {
				Err_DisPlay("Send()");
				break;
			}

			int GetSize;
			char temp[BUFSIZE];		
			User* clientuser;

			GetSize = recv(Client, temp, userlen, 0);
			if (GetSize == SOCKET_ERROR) {
				Err_DisPlay("Recv()");
				break;
			}

			printf("%d\n", GetSize);

			temp[GetSize] = '\0';
			clientuser = (User*)temp;

			for (int i = 0; i < 2; i++) {
				if (!strcmp(user[i].id, clientuser->id) && !strcmp(user[i].pass, clientuser->pass)) {
					strcpy(success, "로그인 성공!");
					flug = true;
					break;
				}
				else {
					strcpy(success, "로그인 실패!");
				}
			}

			retval = send(Client, success, strlen(success), 0);

			if (retval == SOCKET_ERROR) {
				Err_DisPlay("Send()");
				break;
			}
			else if (retval == 0) {
				break;
			}

			while (flug) {
				retval = recv(Client, buf, BUFSIZE, 0);

				if (retval == SOCKET_ERROR) {
					Err_DisPlay("Recv()");
					break;
				}
				else if (retval == 0) {
					break;
				}

				buf[retval] = '\0';
				printf("[받은 데이터] %s\n", buf);

				retval = send(Client, buf, retval, 0);

				if (retval == SOCKET_ERROR) {
					Err_DisPlay("Send()");
					break;
				}
				else if (retval == 0) {
					break;
				}
			}
			break;
		}

		closesocket(Client);
		printf("[TCP 클라이언트 접속 종료] IP : %s , PORT : %d\n", inet_ntoa(ClientServer.sin_addr), ntohs(ClientServer.sin_port));
	}

	closesocket(Listen_Sock);
	WSACleanup();
	return 0;
}