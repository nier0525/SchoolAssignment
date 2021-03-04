#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#include <time.h>

#define SERVERPORT 9000
#define BUFSIZE    4092
#define MSGSIZE 512

#define INTRO_MSG "������ �����մϴ�. 1~100 ���̿� ������ �ֽ��ϴ�.\n\n"
#define UP_MSG "Up!\n"
#define DOWN_MSG "Down\n"
#define WINNER_MSG  " �� ���� ������ ���߼̽��ϴ�!\n"

enum PROTOCOL { INTRO = 1, UP, DOWN, ANSWER_RESULT , WIN};
//enum RESULT { ANSWER_UP , ANSWER_DOWN, ANSWER };

struct _UserInfo {
	int answer;
};

// ���� �Լ� ���� ��� �� ����
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

// ���� �Լ� ���� ���
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

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	return listen_sock;
}

void memcpy_move(int* size, char* ptr, int len) {
	*size += len;
		ptr += len;
}

int main(int argc, char *argv[])
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET listen_sock = sock_init();

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE];
	int answer = 0;

	srand(time(nullptr));

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// ������ Ŭ���̾�Ʈ ���� ���
		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		 answer = rand() % 100 + 1;

		// Ŭ���̾�Ʈ�� ������ ���
		while (1) {
			_UserInfo user;
			ZeroMemory(&user, sizeof(_UserInfo));

			int size = 0;
			char* ptr = buf + sizeof(int);
			char msg[MSGSIZE];
			int strsize = strlen(INTRO_MSG);
			int usersize = 0;
			int count = 0;

			PROTOCOL protocol = INTRO;

			// ----------------------------------------------------------------------------------------------

			memcpy(ptr, &protocol, sizeof(PROTOCOL));
			size += sizeof(PROTOCOL);
			ptr += sizeof(PROTOCOL);

			memcpy(ptr, &strsize, sizeof(int));
			size += sizeof(int);
			ptr += sizeof(int);

			memcpy(ptr, INTRO_MSG, strsize);
			size += strsize;
			ptr = buf;

			memcpy(ptr, &size, sizeof(int));

			retval = send(client_sock, buf, size + sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
			else if (retval == 0) {
				break;
			}

			while (1) {
				bool flug = false;

				size = 0;

				retval = recvn(client_sock, (char*)&size, sizeof(int), 0);
				if (retval == SOCKET_ERROR) {
					err_display("recv()");
					break;
				}
				else if (retval == 0)
					break;

				retval = recvn(client_sock, buf, size, 0);
				if (retval == SOCKET_ERROR) {
					err_display("recv()");
					break;
				}
				else if (retval == 0)
					break;

				memcpy(&protocol, ptr, sizeof(PROTOCOL));
				ptr += sizeof(PROTOCOL);

				switch (protocol) {
				case ANSWER_RESULT:
					memcpy(&usersize, ptr, sizeof(int));
					ptr += sizeof(int);

					memcpy(&user, ptr, usersize);

					if (user.answer < answer) {
						protocol = UP;
					}
					else if (user.answer > answer) {
						protocol = DOWN;
					}
					else if (user.answer == answer) {
						protocol = WIN;
						flug = true;
					}
					break;
				}

				count++;

				switch (protocol) {
				case UP:
					strcpy(msg, UP_MSG);
					break;

				case DOWN:
					strcpy(msg, DOWN_MSG);
					break;

				case WIN:
					sprintf(msg, "%d", count);
					strcat(msg, WINNER_MSG);
					break;
				}

				size = 0;
				ptr = buf + sizeof(int);
				strsize = strlen(msg);

				memcpy(ptr, &protocol, sizeof(PROTOCOL));
				size += sizeof(PROTOCOL);
				ptr += sizeof(PROTOCOL);

				memcpy(ptr, &strsize, sizeof(int));
				size += sizeof(int);
				ptr += sizeof(int);

				memcpy(ptr, msg, strsize);
				size += strsize;
				ptr = buf;

				memcpy(ptr, &size, sizeof(int));

				retval = send(client_sock, buf, size + sizeof(int), 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}
				else if (retval == 0) {
					break;
				}

				ptr = buf;

				if (flug) {
					break;
				}
			}
			break;
		}

		// closesocket()
		closesocket(client_sock);
		printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}

	// closesocket()
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;
}