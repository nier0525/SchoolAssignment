#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 9000
#define BUFSIZE    4092
#define USERSIZE 100
#define NONE 0

#define INTRO_MSG "\n1. ȸ������\n2. �α���\n3. ����\n\n�Է� : "
#define LOGIN_INTRO_MSG "\n1. �α׾ƿ�\n2. ȸ��Ż��\n\n�Է� : "
#define ID_MSG "ID : "
#define PW_MSG "PW : "
#define NEW_MSG "\n���������� ȸ������ �Ǿ����ϴ�.\n"
#define NEW_ERROR_MSG "\n�ߺ��� ���̵��Դϴ�.\n"
#define LOGIN_MSG "\n���������� �α��� �Ǿ����ϴ�.\n"
#define ID_ERROR_MSG "\n���� ID �Դϴ�.\n"
#define PW_ERROR_MSG "\nPW �� Ʋ�Ƚ��ϴ�.\n"
#define OVERRAP_ERROR_MSG "\n���� ���� �����Դϴ�.\n"
#define LOGOUT_MSG "\n���������� �α׾ƿ� �Ǿ����ϴ�.\n"
#define REMOVE_MSG "\nȸ��Ż�� �Ǿ����ϴ�.\n"

enum PROTOCOL
{
	INTRO = 1,
	SELECT,
	NEW,
	LOGIN,
	RESULT,
	LOGIN_INTRO,
	LOGOUT,
	REMOVE,
	RESET,
	EXIT
};

enum STATE
{
	INTRO_STATE = 1,
	SELECT_STATE,
	NEW_STATE,
	LOGIN_STATE,
	LOGIN_INTRO_STATE,
	LOGIN_SELECT_STATE,
	LOGOUT_STATE,
	REMOVE_STATE,
	RESET_STATE,
	EXIT_STATE
};

enum INTRO_MENU
{
	NEW_MENU = 1,
	LOGIN_MENU,
	EXIT_MENU
};

enum LOGIN_MENU
{
	LOGOUT_MENU = 1,
	REMOVE_MENU,
};

struct User_Info {
	char id[128];
	char pw[128];

	bool state;
};

// ���� ���� ������ ���� ����ü�� ����
struct SOCKETINFO
{
	SOCKET sock;
	char buf[BUFSIZE + 1];
	int size;
	int sendbytes;

	User_Info user;
	STATE state;
};

int nTotalSockets = 0;
SOCKETINFO* SocketInfoArray[FD_SETSIZE];

int usercount = 0;
User_Info userlist[USERSIZE];

// ���� ���� �Լ�
BOOL AddSocketInfo(SOCKET sock);
void RemoveSocketInfo(SOCKET sock);

// ���� ��� �Լ�
void err_quit(const char* msg);
void err_display(const char* msg);


PROTOCOL GetProtocol(char* buf) {
	PROTOCOL protocol;
	memcpy(&protocol, buf, sizeof(PROTOCOL));

	return protocol;
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

// Packing
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
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// �ͺ��ŷ �������� ��ȯ
	u_long on = 1;
	retval = ioctlsocket(listen_sock, FIONBIO, &on);
	if (retval == SOCKET_ERROR) err_display("ioctlsocket()");

	// ������ ��ſ� ����� ����
	FD_SET rset, wset;
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen, i;

	char msg[128];
	bool flag;

	while (1) {
		// ���� �� �ʱ�ȭ
		FD_ZERO(&rset);
		FD_ZERO(&wset);
		FD_SET(listen_sock, &rset);

		for (i = 0; i < nTotalSockets; i++) {
			if (SocketInfoArray[i]->size > SocketInfoArray[i]->sendbytes)
				FD_SET(SocketInfoArray[i]->sock, &wset);
			else
				FD_SET(SocketInfoArray[i]->sock, &rset);
		}

		// select()
		retval = select(0, &rset, &wset, NULL, NULL);
		if (retval == SOCKET_ERROR) err_quit("select()");

		// ���� �� �˻�(1): Ŭ���̾�Ʈ ���� ����
		if (FD_ISSET(listen_sock, &rset)) {
			addrlen = sizeof(clientaddr);
			client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
			if (client_sock == INVALID_SOCKET) {
				err_display("accept()");
			}
			else {
				printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
					inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
				// ���� ���� �߰�
				AddSocketInfo(client_sock);
			}
		}

		// ���� �� �˻�(2): ������ ���
		for (i = 0; i < nTotalSockets; i++) {
			SOCKETINFO* ptr = SocketInfoArray[i];

			if (FD_ISSET(ptr->sock, &rset))
			{
				retval = recv(ptr->sock, ptr->buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR)
				{
					err_display("recv()");
					RemoveSocketInfo(ptr->sock);
					continue;
				}

			}

			if (FD_ISSET(ptr->sock, &wset))
			{
				retval = send(ptr->sock, ptr->buf + ptr->sendbytes,
					ptr->size - ptr->sendbytes, 0);
				if (retval == SOCKET_ERROR)
				{
					err_display("send()");
					RemoveSocketInfo(ptr->sock);
					continue;
				}

				ptr->sendbytes += retval;

				if (ptr->size == ptr->sendbytes)
				{
					ptr->size = ptr->sendbytes = 0;
					ZeroMemory(ptr->buf, sizeof(ptr->buf));
				}
			}

			else
			{

				switch (ptr->state) {
				case STATE::INTRO_STATE:
					ptr->size = Packing(ptr->buf, PROTOCOL::INTRO, INTRO_MSG);
					ptr->state = SELECT_STATE;
					break;

				case STATE::SELECT_STATE:
					ZeroMemory(msg, sizeof(msg));
					UnPacking(ptr->buf, msg);
					//printf("%d / %d\n", i + 1, atoi(msg));

					switch (atoi(msg)) {
					case NONE:
						continue;

					case NEW_MENU:
						ptr->size = Packing(ptr->buf, PROTOCOL::NEW, ID_MSG, PW_MSG);
						ptr->state = NEW_STATE;
						break;

					case LOGIN_MENU:
						ptr->size = Packing(ptr->buf, PROTOCOL::LOGIN, ID_MSG, PW_MSG);
						ptr->state = LOGIN_STATE;
						break;

					case EXIT_MENU:
						RemoveSocketInfo(ptr->sock);
						break;

					default:
						ptr->size = Packing(ptr->buf, PROTOCOL::INTRO, INTRO_MSG);
						break;
					}
					break;

				case STATE::NEW_STATE:
					ZeroMemory(&ptr->user, sizeof(ptr->user));
					UnPacking(ptr->buf, ptr->user.id, ptr->user.pw);

					flag = false;

					for (int i = 0; i < usercount; i++) {
						if (!strcmp(userlist[i].id, ptr->user.id)) {
							ptr->size = Packing(ptr->buf, PROTOCOL::RESULT, NEW_ERROR_MSG);
							flag = true;
							break;
						}
					}

					if (!flag) {
						ptr->size = Packing(ptr->buf, PROTOCOL::RESULT, NEW_MSG);
						userlist[usercount++] = ptr->user;
					}
					ptr->state = RESET_STATE;
					break;

				case STATE::LOGIN_STATE:
					ZeroMemory(&ptr->user, sizeof(ptr->user));
					UnPacking(ptr->buf, ptr->user.id, ptr->user.pw);

					flag = false;

					for (int i = 0; i < usercount; i++)
					{
						if (!strcmp(userlist[i].id, ptr->user.id))
						{
							flag = true;
							if (!strcmp(userlist[i].pw, ptr->user.pw))
							{
								if (!userlist[i].state)
								{
									userlist[i].state = true;
									ptr->size = Packing(ptr->buf, PROTOCOL::RESULT, LOGIN_MSG);
									ptr->state = LOGIN_INTRO_STATE;
								}
								else
								{
									ptr->size = Packing(ptr->buf, PROTOCOL::RESULT, OVERRAP_ERROR_MSG);
									ptr->state = RESET_STATE;
								}
							}
							else
							{
								ptr->size = Packing(ptr->buf, PROTOCOL::RESULT, PW_ERROR_MSG);
								ptr->state = RESET_STATE;
							}
							break;
						}
					}

					if (!flag)
					{
						ptr->size = Packing(ptr->buf, PROTOCOL::RESULT, ID_ERROR_MSG);
						ptr->state = RESET_STATE;
					}
					break;

				case STATE::LOGIN_INTRO_STATE:
					ptr->size = Packing(ptr->buf, PROTOCOL::LOGIN_INTRO, LOGIN_INTRO_MSG);
					ptr->state = LOGIN_SELECT_STATE;
					break;

				case STATE::LOGIN_SELECT_STATE:
					ZeroMemory(msg, sizeof(msg));
					UnPacking(ptr->buf, msg);

					switch (atoi(msg)) {
					case NONE:
						continue;

					case LOGOUT_MENU:
						ptr->size = Packing(ptr->buf, PROTOCOL::LOGOUT, LOGOUT_MSG);
						ptr->state = RESET_STATE;
						for (int i = 0; i < usercount; i++) {
							if (!strcmp(userlist[i].id, ptr->user.id)) {
								userlist[i].state = false;
							}
						}
						break;

					case REMOVE_MENU:
						ptr->size = Packing(ptr->buf, PROTOCOL::REMOVE, REMOVE_MSG);
						ptr->state = RESET_STATE;
						for (int i = 0; i < usercount; i++) {
							if (!strcmp(userlist[i].id, ptr->user.id)) {
								for (int j = i; j < usercount - 1; j++) {
									userlist[j] = userlist[j + 1];
								}
								ZeroMemory(&userlist[usercount--], sizeof(User_Info));
								break;
							}
						}
						break;

					default:
						ptr->size = Packing(ptr->buf, PROTOCOL::LOGIN_INTRO, LOGIN_INTRO_MSG);
						break;
					}
					break;

				case STATE::RESET_STATE:
					ptr->size = Packing(ptr->buf, PROTOCOL::INTRO, INTRO_MSG);
					ptr->state = SELECT_STATE;
					break;
				}

			}
		}
	}

	// ���� ����
	WSACleanup();
	return 0;
}

// ���� ���� �߰�
BOOL AddSocketInfo(SOCKET sock)
{
	if (nTotalSockets >= FD_SETSIZE) {
		printf("[����] ���� ������ �߰��� �� �����ϴ�!\n");
		return FALSE;
	}

	SOCKETINFO* ptr = new SOCKETINFO;
	if (ptr == NULL) {
		printf("[����] �޸𸮰� �����մϴ�!\n");
		return FALSE;
	}

	ptr->sock = sock;
	ptr->size = 0;
	ptr->sendbytes = 0;

	ZeroMemory(&ptr->user, sizeof(User_Info));
	ptr->state = INTRO_STATE;

	SocketInfoArray[nTotalSockets++] = ptr;

	return TRUE;
}

// ���� ���� ����
void RemoveSocketInfo(SOCKET sock)
{
	// Ŭ���̾�Ʈ ���� ���
	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);
	getpeername(sock, (SOCKADDR*)&clientaddr, &addrlen);
	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));


	for (int i = 0; i < nTotalSockets; i++) {
		if (SocketInfoArray[i]->sock == sock) {
			closesocket(sock);

			for (int j = i; j < nTotalSockets - 1; j++) {
				SocketInfoArray[j] = SocketInfoArray[j + 1];
			}

			SocketInfoArray[nTotalSockets--] = nullptr;
			break;
		}
	}
}

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