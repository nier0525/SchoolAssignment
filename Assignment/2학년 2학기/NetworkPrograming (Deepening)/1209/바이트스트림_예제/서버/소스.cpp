#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <stdio.h>

#define SERVERPORT 9000
#define BUFSIZE 512

#define ONE_MSG "첫번째 버퍼 데이터입니다."
#define TWO_MSG "두번째 버퍼 데이터입니다."

void err_quit(const char *msg);
void err_display(const char *msg);

SOCKET socket_init();

int main() {
	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return -1;

	SOCKET listen_sock = socket_init();

	SOCKET client_sock;
	SOCKADDR_IN client_addr;
	int addrlen;

	char buf[BUFSIZE];

	while (1) {
		addrlen = sizeof(SOCKADDR_IN);
		client_sock = accept(listen_sock, (SOCKADDR*)&client_addr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("Accept");
			continue;
		}

		printf("[TCP/IP 클라이언트 접속] IP : %s , PORT : %d\n", inet_ntoa(client_addr.sin_addr),
			ntohs(client_addr.sin_port));

		while (1) {
			strcpy(buf, ONE_MSG);
			retval = send(client_sock, buf, strlen(buf), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send");
				break;
			}

			strcpy(buf, TWO_MSG);
			retval = send(client_sock, buf, strlen(buf), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send");
				break;
			}

			break;
		}

		closesocket(client_sock);
		printf("[TCP/IP 클라이언트 종료] IP : %s , PORT : %d\n", inet_ntoa(client_addr.sin_addr),
			ntohs(client_addr.sin_port));
	}

	closesocket(listen_sock);
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

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr,
		sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	return listen_sock;
}
