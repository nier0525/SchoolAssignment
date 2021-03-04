#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "ListNode.h"
#include <time.h>

#define SERVERPORT 9000
#define BUFSIZE    4092
#define MSGSIZE 512

#define INTRO_MSG "������ �����մϴ�. 1~100 ���̿� ������ �ֽ��ϴ�.\n\n"
#define UP_MSG "Up!\n"
#define DOWN_MSG "Down\n"
#define WINNER_MSG  " �� ���� ������ ���߼̽��ϴ�!\n"

enum PROTOCOL { INTRO = 1, UP, DOWN, ANSWER_RESULT, WIN };
//enum RESULT { ANSWER_UP , ANSWER_DOWN, ANSWER };

struct _UserInfo {
	int answer;
};

struct _ClientInfo {
	SOCKET sock;
	SOCKADDR_IN addr;
	char buf[BUFSIZE];

	_UserInfo answer;
};

List<_ClientInfo*>* client_list = init<_ClientInfo*>();

_ClientInfo* AddClientInfo(SOCKET sock, SOCKADDR_IN addr) {
	_ClientInfo* ptr = new _ClientInfo();
	ZeroMemory(ptr, sizeof(_ClientInfo));
	ptr->sock = sock;
	memcpy(&ptr->addr, &addr, sizeof(SOCKADDR_IN));

	// ������ Ŭ���̾�Ʈ ���� ���
	printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

	if (!Insert<_ClientInfo*>(client_list, ptr)) {
		return nullptr;
	};

	return ptr;
}

bool RemoveClientInfo(_ClientInfo* ptr) {
	// closesocket()
	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));

	if (!Delete<_ClientInfo*>(client_list, ptr)) {
		return false;
	};

	closesocket(ptr->sock);
	return true;
}

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
	int strsize = strlen(data);

	ptr += Memcpy_Send(ptr, &protocol, sizeof(int), &size);
	ptr += Memcpy_Send(ptr, &strsize, sizeof(int), &size);
	ptr += Memcpy_Send(ptr, data, strsize, &size);
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

		answer = rand() % 100 + 1;
		_ClientInfo* ptr = AddClientInfo(client_sock, clientaddr);

		// Ŭ���̾�Ʈ�� ������ ���
		while (1) {
			int size = 0;
			char msg[MSGSIZE];

			int count = 0;

			// ----------------------------------------------------------------------------------------------

			size = Packing(ptr->buf, (int*)INTRO, INTRO_MSG);

			retval = send(ptr->sock, ptr->buf, size + sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			while (1) {
				bool flug = false;

				if (!Packing_Recvn(ptr->sock, ptr->buf)) {
					break;
				}

				PROTOCOL protocol = (PROTOCOL)GetProtocol(ptr->buf);

				switch (protocol) {
				case ANSWER_RESULT:
					_UserInfo user;
					ZeroMemory(&user, sizeof(_UserInfo));

					UnPacking(ptr->buf, (char*)&user);

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

				size = Packing(ptr->buf, (int*)protocol, msg);

				retval = send(ptr->sock, ptr->buf, size + sizeof(int), 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}

				if (flug) {
					break;
				}
			}
			break;
		}

		RemoveClientInfo(ptr);
	}

	// closesocket()
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;
}