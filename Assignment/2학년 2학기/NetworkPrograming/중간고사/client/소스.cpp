#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#include <conio.h>

#define SERVERIP   "211.109.106.161"	// ������
#define SERVERPORT 9000		// ��Ʈ ��ȣ
#define BUFSIZE 4092				// ���� ������
#define MSGSIZE 512					// ���ڿ� ������

// �޴� �޼���
#define MAINMENU_MSG "\n< �޴� >\n1.ȸ������\n2.�α���\n3.����\n\n���� : "
#define LOGINMENU_MSG "\n< ���� �߱� ���� >\n1.���ӽ���\n2.ȸ��Ż��\n3.����\n\n���� : "

#define ANSWER 1						// ANSWER ��������

// �������� ������
enum RESULT { ID_MISSING, ID_OVERRAP, PW_MISSING, LOG_NEW, LOGIN_RESULT };
enum PROTOCOL { INTRO = 1, RUN, END };
enum LOGIN_PROTOCOL { NEW = 1, LOGIN , EXIT };
enum GAME_PROTOCOL { START = 1, LOGDEL , LOGOUT };

// ���� ������ �����ϴ� ����ü
struct User {
	char id[20];
	char pw[20];

	int high_score;
	int low_score;
};

// ���� �Լ� ����
void MainMenu(SOCKET);
void LoginMenu(SOCKET);
void NumBaseBall(SOCKET);

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

// ����� ���� ������ ���� �Լ�
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

// ���� �ʱ�ȭ �Լ�
SOCKET sock_init() {
	int retval;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	return sock;
}

// ���� ���� ��ŷ �Լ����� ���ο��� ���� ������ ������ �޸� ���縦 ó���ϰ�, �����ϴ� �������� �����
// �����ؼ� �ּҰ��� ������Ű�� ������ �����Ͽ���

// ���� ���� ��ŷ �Լ�
int memcpy_send(void* ptr, const void* dst, size_t size, int* len) {
	memcpy(ptr, dst, size);		// �޸� ����
	*len += size;							// �޸��� ũ�⸦ ���ڰ����� ���� ũ�⸸ŭ ����
	return size;							// ���ڰ����� ���� ũ�� ����
}

// ���� ���� ����ŷ �Լ�
int memcpy_recv(void* dst, const void* ptr, size_t size) {
	memcpy(dst, ptr, size);		// �޸� ����
	return size;							// ���ڰ����� ���� ũ�� ����
}

// ���� �Լ�
int main() {
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// Ŭ���̾�Ʈ ���� �ʱ�ȭ
	SOCKET sock = sock_init();

	// ���� �޴� �Լ� ����
	MainMenu(sock);

	// closesocket()
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}

// ���� �޴� �Լ�
void MainMenu(SOCKET sock) {
	int retval;

	User user;	// ���� ������ ������ ����
	ZeroMemory(&user, sizeof(User));	// �ʱ�ȭ �۾�

	char buf[BUFSIZE];
	char msg[MSGSIZE];

	while (1) {
		int size = 0;
		char* ptr = buf;
		int len = 0;

		PROTOCOL protocol;
		LOGIN_PROTOCOL sel;
		RESULT result;

		// �޴� ���
		printf("%s", MAINMENU_MSG);

		// �޴����� ������ �� �Է�
		scanf("%s", msg);

		// 1 �ڸ� �̿��� �ڸ��� �ʿ� �����Ƿ� ���� ó��
		msg[1] = '\0';

		// �Է��� ���ڿ��� �������ݷ� ĳ���� ��, �������� ������ ����
		sel = (LOGIN_PROTOCOL)atoi(msg);

		// �������� ���� �۽�
		retval = send(sock, (char*)&sel, sizeof(LOGIN_PROTOCOL), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send");
			break;
		}

		// �������� ������ ���� �۾�
		switch (sel) {
		case NEW:
		case LOGIN:
			// ���� ���� ������ ������ ����
			retval = recvn(sock, (char*)&size, sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("recvn");
				return;
			}
			else if (retval == 0) {
				return;
			}

			// ���� ����
			retval = recvn(sock, buf, size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recvn");
				return;
			}
			else if (retval == 0) {
				return;
			}

			// �������� ���� ����ŷ
			ptr += memcpy_recv(&protocol, ptr, sizeof(PROTOCOL));

			// �������� ������ ���� �۾�
			switch (protocol) {
			case INTRO:
				// �޼��� ���� ����ŷ
				ptr += memcpy_recv(&len, ptr, sizeof(int));
				ptr += memcpy_recv(&msg, ptr, len);

				// ����ŷ�� �޼��� ���� ���
				msg[len] = '\0';
				printf("%s", msg);
				break;
			}

			// ������ ID, PW ���� �Է�
			printf("\nID : ");
			scanf("%s", user.id);
			printf("\nPASSWORD : ");
			scanf("%s", user.pw);
			break;

		case EXIT:
			// �Լ� ����
			return;

		default:
			// �������� ���� ���̸� ó������
			printf("\n\n�߸��� �Է��Դϴ�.\n");
			if (getch()) {
				system("cls");
				continue;
			}
			break;
		}

		ptr = buf + sizeof(int);
		len = sizeof(User);

		// ��������, ���� ������ ��ŷ
		ptr += memcpy_send(ptr, &sel, sizeof(LOGIN_PROTOCOL), &size);
		ptr += memcpy_send(ptr, &len, sizeof(int), &size);
		ptr += memcpy_send(ptr, (char*)&user, len, &size);
		ptr = buf;

		// ������ ũ�⸦ ��ŷ
		memcpy(ptr, &size, sizeof(int));

		// ���� �۽�
		retval = send(sock, buf, size + sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send");
			break;;
		}

		// ������ ������ ũ�⸦ ����
		retval = recvn(sock, (char*)&size, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("recvn");
			return;
		}
		else if (retval == 0) {
			return;
		}

		// ���� ����
		retval = recvn(sock, buf, size, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recvn");
			return;
		}
		else if (retval == 0) {
			return;
		}

		// ��������, �޽��� ������ ����ŷ
		ptr += memcpy_recv(&result, ptr, sizeof(RESULT));
		ptr += memcpy_recv(&len, ptr, sizeof(int));
		ptr += memcpy_recv(&msg, ptr, len);

		// ����ŷ�� �޽����� ������ ���
		msg[len] = '\0';
		printf("%s", msg);

		// �������ݿ� ���� �۾�
		switch (result) {
		case LOGIN_RESULT:
			// �α��� �޴� �Լ� ����
			if (getch()) {
				system("cls");
				LoginMenu(sock);
			}
			break;
		default:
			// �α��� �����̱� ������ ó������
			if (getch()) {
				system("cls");
			}
			break;
		}
	}
}

// �α��� �޴� �Լ�
void LoginMenu(SOCKET sock) {
	int retval;

	char buf[BUFSIZE];
	char msg[MSGSIZE];

	while (1) {
		int size = 0;
		char* ptr = buf;
		int len = 0;

		GAME_PROTOCOL sel;

		// �α��� �޴� ���
		printf("%s", LOGINMENU_MSG);
		
		// ������ �޴��� �Է�
		scanf("%s", msg);

		// 1 �ڸ� �̿ܿ��� �ʿ� �����Ƿ�, ���� ó��
		msg[1] = '\0';
		
		// �Է��� ���ڿ��� �������ݷ� ĳ���� ����, �������� ������ ����
		sel = (GAME_PROTOCOL)atoi(msg);

		// �������� ���� �۽�
		retval = send(sock, (char*)&sel, sizeof(GAME_PROTOCOL), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send");
			break;
		}

		// �������� ������ ���� �۾�
		switch (sel) {
		case START:
			// ���� �߱� ���� �Լ� ����
			NumBaseBall(sock);
			break;

		case LOGDEL:
		case LOGOUT:
			// ������ ������ ũ�⸦ ����
			retval = recvn(sock, (char*)&size, sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("recvn");
				return;
			}
			else if (retval == 0) {
				return;
			}

			// ���� ����
			retval = recvn(sock, buf, size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recvn");
				return;
			}
			else if (retval == 0) {
				return;
			}

			// ��������, �޼��� ���� ����ŷ
			ptr += memcpy_recv(&sel, ptr, sizeof(GAME_PROTOCOL));
			ptr += memcpy_recv(&len, ptr, sizeof(int));
			ptr += memcpy_recv(&msg, ptr, len);

			// ����ŷ�� �޽��� ������ ���
			msg[len] = '\0';
			printf("%s", msg);

			if (getch()) {
				system("cls");
			}
			return;
		}
	}
}

// ���� �߱� ���� �Լ�
void NumBaseBall(SOCKET sock) {
	int retval;
	int answer_protocol = ANSWER;

	char buf[BUFSIZE];
	char msg[MSGSIZE];

	int count;

	while (1) {
		int size = 0;
		char* ptr = buf;
		int len = 0;
		PROTOCOL protocol;

		// ������ ������ ũ�⸦ ����
		retval = recvn(sock, (char*)&size, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("recvn");
			return;
		}
		else if (retval == 0) {
			return;
		}

		// ���� ����
		retval = recvn(sock, buf, size, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recvn");
			return;
		}
		else if (retval == 0) {
			return;
		}

		// �������� ���� ����ŷ
		ptr += memcpy_recv(&protocol, ptr, sizeof(PROTOCOL));

		// �������� ������ ���� �۾�
		switch (protocol) {
		case INTRO:
			// �޼��� ���� ����ŷ
			ptr += memcpy_recv(&len, ptr, sizeof(int));
			ptr += memcpy_recv(&msg, ptr, len);

			msg[len] = '\0';
			printf("%s", msg);

			ptr += memcpy_recv(&len, ptr, sizeof(int));

			count = len;

			for (int i = 0; i < count; i++) {
				ptr += memcpy_recv(&len, ptr, sizeof(int));
				ptr += memcpy_recv(&msg, ptr, len);

				msg[len] = '\0';
				printf("%s", msg);
			}

			ptr += memcpy_recv(&len, ptr, sizeof(int));
			ptr += memcpy_recv(&msg, ptr, len);

			// ����ŷ�� �޽��� ������ ���
			msg[len] = '\0';
			printf("%s", msg);
			continue;

		case RUN:
			//�޼��� ���� ����ŷ
			ptr += memcpy_recv(&len, ptr, sizeof(int));
			ptr += memcpy_recv(&msg, ptr, len);

			// ����ŷ�� �޽��� ������ ���
			msg[len] = '\0';
			printf("%s", msg);

			// ������ �Է�
			scanf("%s", msg);

			size = 0;
			ptr = buf + sizeof(int);
			len = strlen(msg);

			// ��������, �޽��� ���� ��ŷ
			ptr += memcpy_send(ptr, &answer_protocol, sizeof(ANSWER), &size);
			ptr += memcpy_send(ptr, &len, sizeof(int), &size);
			ptr += memcpy_send(ptr, &msg, len, &size);
			ptr = buf;

			// ������ ũ�� ��ŷ
			memcpy(ptr, &size, sizeof(int));

			// ���� �۽�
			retval = send(sock, buf, size + sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send");
				return;
			}

			// ������ ������ ũ�⸦ ����
			retval = recvn(sock, (char*)&size, sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("recvn");
				return;
			}
			else if (retval == 0) {
				return;
			}

			// ���� ����
			retval = recvn(sock, buf, size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recvn");
				return;
			}
			else if (retval == 0) {
				return;
			}

			// �������� ���� ����ŷ
			ptr += memcpy_recv(&answer_protocol, ptr, sizeof(ANSWER));

			// �������� ������ ���� �۾�
			switch (answer_protocol) {
			case ANSWER:
				// �޽��� ���� ����ŷ
				ptr += memcpy_recv(&len, ptr, sizeof(int));
				ptr += memcpy_recv(&msg, ptr, len);

				// ����ŷ�� �޽��� ������ ���
				msg[len] = '\0';
				printf("%s", msg);
				break;
			}
			continue;

		case END:
			// �޼��� ���� ����ŷ
			ptr += memcpy_recv(&len, ptr, sizeof(int));
			ptr += memcpy_recv(&msg, ptr, len);

			// ����ŷ�� �޽��� ������ ���
			msg[len] = '\0';
			printf("\n%s", msg);
			
			if (getch()) {
				system("cls");
			}
			return;
		}
	}
}
