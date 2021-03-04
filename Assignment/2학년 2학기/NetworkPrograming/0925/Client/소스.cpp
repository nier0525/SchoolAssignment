#pragma comment (lib,"ws2_32")
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512

struct User {
	char id[10];
	char pass[10];
}user;


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

SOCKET SockInit() {
	int retval;

	SOCKET Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (Sock == INVALID_SOCKET) {
		Err_Quit("socket()");
	}

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);

	retval = connect(Sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	if (retval == SOCKET_ERROR) {
		Err_Quit("Connect()");
	}

	return Sock;
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

	SOCKET Sock = SockInit();

	User user;

	char buf[BUFSIZE + 1];

	char log[BUFSIZE];

	int len;
	int userlen;

	bool flug = false;

	userlen = sizeof(user);
	retval = send(Sock, (char*)&userlen, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		Err_Quit("Send()");
	}

	retval = recv(Sock, log, BUFSIZE, 0);
	if (retval == SOCKET_ERROR) {
		Err_Quit("Recv()");
	}
	log[retval] = '\0';

	printf("%s", log);

	if (fgets(user.id, 10, stdin) == NULL) {
		flug = true;
	}

	retval = recv(Sock, log, BUFSIZE, 0);
	if (retval == SOCKET_ERROR) {
		Err_Quit("Recv()");
	}
	log[retval] = '\0';

	printf("%s", log);

	if (fgets(user.pass, 10, stdin) == NULL) {
		flug = true;
	}

	user.id[strlen(user.id) - 1] = '\0';
	user.pass[strlen(user.pass) - 1] = '\0';

	retval = send(Sock, (char*)&user, sizeof(User), 0);
	if (retval == SOCKET_ERROR) {
		Err_Quit("Send()");

	}
	else if (retval == 0) {
		flug = true;
	}


	retval = recv(Sock, log, BUFSIZE, 0);
	if (retval == SOCKET_ERROR) {
		Err_Quit("Recv()");
	}

	log[retval] = '\0';
	printf("%s\n", log);

	if (!strcmp(log, "로그인 실패!")) {
		flug = true;
	}

	while (!flug) {
		// 데이터 입력
		printf("\n[보낼 데이터] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
			break;

		// '\n' 문자 제거
		len = strlen(buf);
		if (buf[len - 1] == '\n')
			buf[len - 1] = '\0';
		if (strlen(buf) == 0)
			break;

		// 데이터 보내기
		retval = send(Sock, buf, strlen(buf), 0);
		if (retval == SOCKET_ERROR) {
			Err_DisPlay("recv()");
			break;
		}
		printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);

		// 데이터 받기
		retval = recvn(Sock, buf, retval, 0);
		if (retval == SOCKET_ERROR) {
			Err_DisPlay("recv()");
			break;
		}
		else if (retval == 0)
			break;

		buf[retval] = '\0';
		printf("[TCP 클라이언트] %d바이트를 받았습니다.\n", retval);
		printf("[받은 데이터] %s\n", buf);
	}

	closesocket(Sock);
	WSACleanup();
	return 0;
}