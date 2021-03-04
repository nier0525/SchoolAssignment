#pragma comment (lib, "ws2_32")
#include <WinSock2.h>

#include <stdio.h>
#include <stdlib.h>

#define SERVERPORT 9000
#define BUFSIZE 4096
#define MSGSIZE 512

#define INTRO_MSG "ID �� PW �� �Է��ϼ���.\n"
#define ID_ERROR_MSG "���� ID �Դϴ�.\n"
#define PW_ERROR_MSG "PW �� ��ġ���� �ʽ��ϴ�.\n"
#define LOGIN_MSG "�α��ο� �����߽��ϴ�.\n"

// ��������
enum RESULT { ID_ERROR = 1, PW_ERROR, LOGIN };
enum PROTOCOL { INTRO = 1, LOGIN_INFO, LOGIN_RESULT };

// ���� ����
struct User {
	char id[30];
	char pw[30];
};

struct Client_Info {
	SOCKET sock;
	SOCKADDR_IN addr;
	char buf[BUFSIZE];

	User user;
};

User user_list[3] = { {"aaa","111"} , {"bbb", "222"} , {"kja0204", "1234"} };
Client_Info* client_info[20];
int client_count = 0;

// ���� �Լ�
Client_Info* Add_ClientInfo(SOCKET sock, SOCKADDR_IN addr);
bool Remove_ClientInfo(Client_Info* ptr);

int recvn(SOCKET sock, char* buf, int len, int flug);
void err_quit(const char* msg);
void err_display(const char* msg);

SOCKET SocketInit();

int memcpy_send(void* ptr, const void* dst, size_t size, int* len);
int memcpy_recv(void* dst, const void* ptr, size_t size);

bool Packing_Recv(SOCKET sock, char* buf);
int GetProtocol(char* buf);

int Packing(char* buf, const int protocol, const char* msg);
int Packing(char* buf, const int protocol, const int result, const char* msg);

void UnPacking(const char* buf, char* msg, char* msg2);

DWORD WINAPI LoginSystem(LPVOID arg);

// ���� ������
int main() {
	WSADATA wsa;
	if (WSAStartup(MAKELONG(2, 2), &wsa) != 0) {
		err_quit("WSAStartup");
	}

	SOCKET listen_sock = SocketInit();

	SOCKET client_sock;
	SOCKADDR_IN client_addr;
	int addrlen;
	
	while (1) {
		addrlen = sizeof(SOCKADDR_IN);
		client_sock = accept(listen_sock, (SOCKADDR*)&client_addr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept");
			continue;
		}

		Client_Info* ptr = Add_ClientInfo(client_sock, client_addr);

		HANDLE hThread = CreateThread(nullptr, 0, LoginSystem, (LPVOID)ptr, 0, nullptr);

		if (hThread == nullptr) {
			Remove_ClientInfo(ptr);
		}
		else {
			CloseHandle(hThread);
		}
	}

	closesocket(listen_sock);
	WSACleanup();
	return 0;
}

// ���� Ŭ���̾�Ʈ ���� �߰�
Client_Info* Add_ClientInfo(SOCKET sock, SOCKADDR_IN addr) {
	Client_Info* ptr = new Client_Info;
	ZeroMemory(ptr, sizeof(Client_Info));

	ptr->sock = sock;
	memcpy(&ptr->addr, &addr, sizeof(SOCKADDR_IN));

	printf("[ Ŭ���̾�Ʈ ���� ] IP : %s , PORT : %d\n", inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));
	client_info[client_count++] = ptr;
	return ptr;
}

// ���� Ŭ���̾�Ʈ ���� ����
bool Remove_ClientInfo(Client_Info* ptr) {
	printf("[ Ŭ���̾�Ʈ ���� ] IP : %s , PORT : %d\n", inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));
	closesocket(ptr->sock);

	for (int i = 0; i < client_count; i++) {
		if (client_info[i] == ptr) {
			delete client_info[i];
			for (int j = i; j < client_count - 1; j++) {
				client_info[j] = client_info[j + 1];
			}
			ZeroMemory(client_info[client_count - 1], sizeof(Client_Info));
			client_count--;
			return true;
		}
	}

	return false;
}

// ����� ���� Recv �Լ�
int recvn(SOCKET sock, char* buf, int len, int flug) {
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(sock, ptr, left, flug);

		if (received == SOCKET_ERROR) {
			return SOCKET_ERROR;
		}
		else if (received == 0) {
			break;
		}

		left -= received;
		ptr += received;
	}

	return (len - left);
}

// ���� üũ ( ���� )
void err_quit(const char* msg) {
	LPVOID lpmsgbuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&lpmsgbuf, 0, NULL);

	MessageBox(nullptr, (LPCTSTR)lpmsgbuf, msg, MB_ICONERROR);
	LocalFree(lpmsgbuf);
	exit(1);
}

// ���� üũ ( ��� )
void err_display(const char* msg) {
	LPVOID lpmsgbuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&lpmsgbuf, 0, NULL);

	printf("[%s] %s\n", msg, (LPCSTR)lpmsgbuf);
	LocalFree(lpmsgbuf);
}

// ���� �ʱ�ȭ �Լ�
SOCKET SocketInit() {
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		err_quit("socket");
	}

	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(SOCKADDR_IN));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(SERVERPORT);

	int retval = bind(sock, (SOCKADDR*)&addr, sizeof(addr));
	if (retval == SOCKET_ERROR) {
		err_quit("bind");
	}

	retval = listen(sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) {
		err_quit("listen");
	}

	return sock;
}

// ��ŷ �޸� ���� �Լ�
int memcpy_send(void* ptr, const void* dst, size_t size, int* len) {
	memcpy(ptr, dst, size);
	*len += size;
	return size;
}

// ����ŷ �޸� ���� �Լ�
int memcpy_recv(void* dst, const void* ptr, size_t size) {
	memcpy(dst, ptr, size);
	return size;
}

// ��ŷ Recv �Լ�
bool Packing_Recv(SOCKET sock, char* buf) {
	int retval;
	int size;

	retval = recvn(sock, (char*)&size, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recvn");
		return false;
	}
	else if (retval == 0) {
		return false;
	}

	retval = recvn(sock, buf, size, 0);
	if (retval == SOCKET_ERROR) {
		err_display("recvn");
		return false;
	}
	else if (retval == 0) {
		return false;
	}

	return true;
}

// �������� ȣ�� �Լ�
int GetProtocol(char* buf) {
	int protocol;
	memcpy(&protocol, buf, sizeof(int));
	return protocol;
}

// ��ŷ �Լ� ( char )
int Packing(char* buf, const int protocol, const char* msg) {
	int size = 0;
	char* ptr = buf + sizeof(int);
	int len = strlen(msg);

	ptr += memcpy_send(ptr, &protocol, sizeof(int), &size);
	ptr += memcpy_send(ptr, &len, sizeof(int), &size);
	ptr += memcpy_send(ptr, msg, len, &size);

	ptr = buf;

	memcpy(ptr, &size, sizeof(int));

	return size;
}

// ��ŷ �Լ� ( result, char )
int Packing(char* buf, const int protocol, const int result, const char* msg) {
	int size = 0;
	char* ptr = buf + sizeof(int);
	int len = strlen(msg);

	ptr += memcpy_send(ptr, &protocol, sizeof(int), &size);
	ptr += memcpy_send(ptr, &result, sizeof(int), &size);
	ptr += memcpy_send(ptr, &len, sizeof(int), &size);
	ptr += memcpy_send(ptr, msg, len, &size);

	ptr = buf;

	memcpy(ptr, &size, sizeof(int));

	return size;
}

// ����ŷ �Լ� ( ����ü )
void UnPacking(const char* buf, char* msg , char* msg2) {
	const char* ptr = buf + sizeof(int);
	int size = strlen(msg);
	int size2 = strlen(msg2);

	ptr += memcpy_recv(&size, ptr, sizeof(int));
	ptr += memcpy_recv(msg, ptr, size);
	ptr += memcpy_recv(&size2, ptr, sizeof(int));
	ptr += memcpy_recv(msg2, ptr, size2);
}

// �α��� ������ �Լ�
DWORD WINAPI LoginSystem(LPVOID arg) {
	Client_Info* ptr = (Client_Info*)arg;

	while (1) {
		int size = Packing(ptr->buf, (int)INTRO, INTRO_MSG);

		int retval = send(ptr->sock, ptr->buf, size + sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send");
			break;
		}

		if (!Packing_Recv(ptr->sock, ptr->buf)) {
			break;
		}

		PROTOCOL protocol = (PROTOCOL)GetProtocol(ptr->buf);

		User nowuser;

		bool flug = false;
		RESULT result = ID_ERROR;
		char msg[MSGSIZE];

		switch (protocol) {
		case LOGIN_INFO:
			ZeroMemory(&nowuser, sizeof(User));
			UnPacking(ptr->buf, nowuser.id, nowuser.pw);

			for (int i = 0; i < 3; i++) {
				if (!strcmp(user_list[i].id, nowuser.id)) {
					flug = true;
					if (!strcmp(user_list[i].pw, nowuser.pw)) {
						memcpy(&ptr->user, &nowuser, sizeof(User));

						result = LOGIN;
						strcpy(msg, LOGIN_MSG);
					}
					else {
						result = PW_ERROR;
						strcpy(msg, PW_ERROR_MSG);
					}
					break;
				}
			}

			if (!flug) {
				result = ID_ERROR;
				strcpy(msg, ID_ERROR_MSG);
			}

			size = Packing(ptr->buf, (int)LOGIN_RESULT, result, msg);
			retval = send(ptr->sock, ptr->buf, size + sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send");
				break;
			}
			break;
		}

		if (result == LOGIN) {
			break;
		}
	}

	Remove_ClientInfo(ptr);
	return 0;
}

