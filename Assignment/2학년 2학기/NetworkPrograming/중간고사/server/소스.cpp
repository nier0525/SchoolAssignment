#pragma comment (lib, "ws2_32")
#include <WinSock2.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// �ְ�, ���� ������ �߷����� ���� �Լ� ��ũ��
#define max(a,b) ((a  > b) ? a : b)
#define min(a,b) ((a < b) ? a : b)

#define SERVERPORT 9000	// ��Ʈ ��ȣ
#define BUFSIZE 4092			// ���� ������
#define MSGSIZE 512				// ���ڿ� ������
#define USERSIZE 100			// ���� ������ ������ �迭�� ũ��

#define ANSWER 1					// ANSWER ��������

// Ŭ���̾�Ʈ�� ���� �޼���
#define LOG_INTRO_MSG "\n\nID �� PASSWORD �� �Է��ϼ���\n"
#define GAME_INTRO_MSG "\n���� �߱� ������ �����մϴ�. ������ 1~9 ���� �Դϴ�.\n"

#define GAME_INFO_MSG "\n3 ���� ���ڸ� �Է��ϼ���. ( �� : 456 , 174 ��� )\n�Է� : "
#define GAME_OVERRAP_MSG "\n�Էµ� �� �� �ߺ��� ���� ������ �ȵ˴ϴ�.\n"
#define GAME_OVERHEAD_MSG "\n�Էµ� ���� �߸��� �����Դϴ�.\n"
#define GAME_END_MSG "\n\n���ӿ��� �¸��ϼ̽��ϴ�!\n"

#define ID_MISSING_MSG "\n����  ID �Դϴ�.\n"
#define ID_OVERRAP_MSG "\n�ߺ��� ID �Դϴ�.\n"
#define PW_MISSING_MSG "\nPASSWORD �� Ʋ�Ƚ��ϴ�.\n"

#define NEW_MSG "\n���������� ȸ������ �Ǿ����ϴ�.\n"
#define LOGIN_MSG "\n���������� �α��� �Ǿ����ϴ�.\n"
#define LOGOUT_MSG "\n���������� �α׾ƿ� �Ǿ����ϴ�.\n"
#define DELETE_MSG "\n���������� ȸ��Ż�� �Ǿ����ϴ�.\n"

// �������� ������
enum RESULT { ID_MISSING , ID_OVERRAP, PW_MISSING, LOG_NEW ,LOGIN_RESULT };
enum PROTOCOL { INTRO = 1 , RUN , END };
enum LOGIN_PROTOCOL { NEW = 1, LOGIN, EXIT };
enum GAME_PROTOCOL { START = 1, LOGDEL, LOGOUT };

// ���� ������ �����ϴ� ����ü
struct User {
	char id[20];
	char pw[20];

	int high_score;
	int low_score;
};

User userlist[USERSIZE];	// ���� ������ ������ �迭
int count = 0;

// ���� �Լ� ����
bool FileSave(User*, int);
bool FileLoad(User*, int*);

void MainMenu(SOCKET);
void LoginMenu(SOCKET, User);
void NumBaseBall(SOCKET, User);

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

// ���� ���� ��ŷ �Լ����� ���ο��� ���� ������ ������ �޸� ���縦 ó���ϰ�, �����ϴ� �������� �����
// �����ؼ� �ּҰ��� ������Ű�� ������ �����Ͽ���

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

int main() {
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ���� ���� �ʱ�ȭ
	SOCKET listen_sock = sock_init();

	// ������ ��ſ� ����� ����
	SOCKET client_sock;		// Ŭ���̾�Ʈ ����
	SOCKADDR_IN clientaddr;		// Ŭ���̾�Ʈ �ּ� ����
	int addrlen;						// ũ�⸦ ������ input & output ������ ���� ����

	// ���� �迭 �ʱ�ȭ
	for (int i = 0; i < USERSIZE; i++) {
		ZeroMemory(&userlist[i], sizeof(User));
	}
	// ���� ������ ���� ����

	// ������ �� �ܺ��� ���Ͽ� ����� ���� ������ �о��
	FileLoad(userlist, &count);

	// rand �� �ߺ� ������ ���� srand
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
		printf("\n[TCP ����] ���� ��ϵ� ����\n");
		for (int i = 0; i < count; i++) {
			printf("[ Log ] ID : %s , PW : %s\n", userlist[i].id, userlist[i].pw);
		}

		// ���� �޴� �Լ� ����
		MainMenu(client_sock);

		// closesocket()
		closesocket(client_sock);
		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}
	// closesocket()
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;
}

// ���� �޴� �Լ�
void MainMenu(SOCKET sock) {
	int retval;

	char buf[BUFSIZE];
	char msg[MSGSIZE];

	User clientuser;	// Ŭ���̾�Ʈ ���� ������ ������ ����
	ZeroMemory(&clientuser, sizeof(User));	// �ʱ�ȭ �۾�

	while (1) {
		int size = 0;
		char* ptr = buf + sizeof(int);
		PROTOCOL protocol = INTRO;
		LOGIN_PROTOCOL login_protocol;
		RESULT result;
		int len = strlen(LOG_INTRO_MSG);

		// �������� ���� ����
		retval = recvn(sock, (char*)&login_protocol, sizeof(LOGIN_PROTOCOL), 0);
		if (retval == SOCKET_ERROR) {
			err_display("recvn");
			break;
		}
		else if (retval == 0) {
			break;
		}

		// �������� ������ ���� �۾�
		switch (login_protocol) {
		case NEW:
		case LOGIN:
			// ��������, �޼��� ���� ��ŷ
			ptr += memcpy_send(ptr, &protocol, sizeof(PROTOCOL), &size);
			ptr += memcpy_send(ptr, &len, sizeof(int), &size);
			ptr += memcpy_send(ptr, LOG_INTRO_MSG, len, &size);
			ptr = buf;

			// ������ ũ�� ��ŷ
			memcpy(ptr, &size, sizeof(int));

			// ���� �۽�
			retval = send(sock, buf, size + sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send");
				return;
			}
			break;


		case EXIT:
			// �Լ� ����
			return;
			break;

		default:
			// �������� ������ �ʰ����� ���, ó������ �ٽ� �۾�
			continue;
			break;
		}

		// ������ ������ ũ�� ����
		retval = recvn(sock, (char*)&size, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("recvn");
			break;
		}
		else if (retval == 0) {
			break;
		}

		// ���� ����
		retval = recvn(sock, buf, size, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recvn");
			break;
		}
		else if (retval == 0) {
			break;
		}

		// ��������, Ŭ���̾�Ʈ ���� ������ ����ŷ
		ptr += memcpy_recv(&login_protocol, ptr, sizeof(LOGIN_PROTOCOL));
		ptr += memcpy_recv(&len, ptr, sizeof(int));
		ptr += memcpy_recv(&clientuser, ptr, len);

		// ȸ������, �α��� �۾��� �ʿ��� ����
		bool flug = false;
		bool login = false;

		// �������� ������ ���� �۾�
		switch (login_protocol) {
		case NEW:
			// ���� ���� ���� �� ID �ߺ� üũ
			for (int i = 0; i < count; i++) {
				if (!strcmp(userlist[i].id, clientuser.id)) {
					result = ID_OVERRAP;
					strcpy(msg, ID_OVERRAP_MSG);

					flug = true;
					break;
				}
			}

			// ID �ߺ��� ���ٸ� ���� ���� ����
			if (!flug) {
				result = LOG_NEW;
				strcpy(msg, NEW_MSG);

				userlist[count] = clientuser;
				printf("[ New Log ] ID : %s , PW : %s\n", userlist[count].id, userlist[count].pw);
				count += 1;

				// ���ŵ� ������ ���� ���Ͽ� ����
				FileSave(userlist, count);
			}
			break;

		case LOGIN:
			// Ŭ���̾�Ʈ�� �Է��� ������ ������ ���� �迭�� �ִ� �� �˻�
			for (int i = 0; i < count; i++) {
				if (!strcmp(userlist[i].id, clientuser.id)) {
					flug = true;
					if (!strcmp(userlist[i].pw, clientuser.pw)) {
						result = LOGIN_RESULT;
						strcpy(msg, LOGIN_MSG);
						login = true;
					}
					else {
						result = PW_MISSING;
						strcpy(msg, PW_MISSING_MSG);
					}
				}
			}

			// ���̵� ��ġ���� ���� ��� ( ���� �迭�� ������ ���� )
			if (!flug) {
				result = ID_MISSING;
				strcpy(msg, ID_MISSING_MSG);
			}
			break;
		}

		size = 0;
		ptr = buf + sizeof(int);
		len = strlen(msg);

		// ��������, �޼��� ���� ��ŷ
		ptr += memcpy_send(ptr, &result, sizeof(RESULT), &size);
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

		// �α��ο� �����ߴٸ� �α��� �޴� �Լ� ����
		if (login) {
			LoginMenu(sock, clientuser);
		}
	}
}

// �α��� �޴� �Լ�
void LoginMenu(SOCKET sock, User clientuser) {
	int retval;

	// ����,�۽ſ� ���� ����
	char buf[BUFSIZE];
	char msg[MSGSIZE];

	while (1) {
		int size = 0;
		char* ptr = buf + sizeof(int);
		GAME_PROTOCOL game_protocol;
		int len = 0;

		// �������� ���� ����
		retval = recvn(sock, (char*)&game_protocol, sizeof(GAME_PROTOCOL), 0);
		if (retval == SOCKET_ERROR) {
			err_display("recvn");
			break;
		}
		else if (retval == 0) {
			break;
		}

		// �������� ������ ���� �۾�
		switch (game_protocol) {
		case START:
			// ���� �߱� ���� �Լ� ����
			NumBaseBall(sock, clientuser);
			continue;

		case LOGDEL:
			// ���� ����Ʈ �迭 ������ ������ Ŭ���̾�Ʈ�� ���� ������ ���� �۾�
			for (int i = 0; i < count; i++) {
				if (!strcmp(userlist[i].id, clientuser.id) && !strcmp(userlist[i].pw, clientuser.pw)) {
					printf("[ Delete Log ] ID : %s , PW : %s\n", userlist[i].id, userlist[i].pw);
					for (int j = i; j < count - 1; j++) {	// ���� ����, �� �ڸ��� ä���ִ� �۾�
						userlist[j] = userlist[j + 1];
					}
					ZeroMemory(&userlist[count - 1], sizeof(User));
					count -= 1;
				}
			}

			// �޼����� �������� ���� ����
			strcpy(msg, DELETE_MSG);
			game_protocol = LOGDEL;

			// �ܺο� ���Ͽ� ���� ������ ���� ����Ʈ �迭�� ������ ����
			FileSave(userlist, count);
			break;

		case LOGOUT:
			// ���� Ŭ���̾�Ʈ�� ������ �ְ�,������ ����� ���
			for (int i = 0; i < count; i++) {
				if (!strcmp(userlist[i].id, clientuser.id) && !strcmp(userlist[i].pw, clientuser.pw)) {
					sprintf(msg, "\n���� ������ ���� �ִ� ����� %d �̰�, ���� ����� %d �Դϴ�.\n", userlist[i].high_score, userlist[i].low_score);
				}
			}
			// �޼����� �������� ���� ����
			strcat(msg, LOGOUT_MSG);
			game_protocol = LOGOUT;
			break;
		}

		len = strlen(msg);

		// ��������, �޼����� ������ ��ŷ
		ptr += memcpy_send(ptr, &game_protocol, sizeof(GAME_PROTOCOL), &size);
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
		break;
	}
}

// ���� �߱� ���� �Լ�
void NumBaseBall(SOCKET sock, User clientuser) {
	int retval;
	int answer_protocol;	// answer ��������

	int number[3];				// ������ ������ �迭
	int answer[3];				// �Է� ���� ���� ������ �迭

	int index = 0;				// ���� ���� Ƚ��

	// ����,�۽ſ� ���� ����
	char buf[BUFSIZE];
	char msg[MSGSIZE];

	PROTOCOL protocol = INTRO;
	int len = strlen(GAME_INTRO_MSG);	

	// ���� �߱� ������ ������ ���������� �ʱ�ȭ �����ִ� �۾�
	// for ���� �� �� ���� ������ �ߺ��Ǵ� ���� ������ �ʱ�ȭ
	for (int i = 0; i < 3; i++) {
		number[i] = rand() % 9 + 1;
		for (int j = 0; j < i; j++) {
			if (number[i] == number[j]) {
				i--;
				break;
			}
		}
	}

	while (1) {
		// ����, �۽ſ� ���� ����
		int size = 0;
		char* ptr = buf + sizeof(int);

		// �������� ���� ��ŷ
		ptr += memcpy_send(ptr, &protocol, sizeof(PROTOCOL), &size);

		// ���� �������� ������ ����� �������� ���� ���� �۾��� ����
		switch (protocol) {
		case INTRO:
			// �޼��� ���� ��ŷ
			sprintf(msg, "\n[ ���� �߱� ��ŷ ��Ȳ ] \n--------------------------------------------- \n");
			len = strlen(msg);

			ptr += memcpy_send(ptr, &len, sizeof(int), &size);
			ptr += memcpy_send(ptr, msg, len, &size);
			
			len = count;

			ptr += memcpy_send(ptr, &len, sizeof(int), &size);

			for (int i = 0; i < count; i++) {
				sprintf(msg, "ID : %s  /  BestScore : %d  /  WorstScore  :  %d\n", userlist[i].id, userlist[i].high_score, userlist[i].low_score);
				len = strlen(msg);

				ptr += memcpy_send(ptr, &len, sizeof(int), &size);
				ptr += memcpy_send(ptr, msg, len, &size);
			}

			ptr += memcpy_send(ptr, &len, sizeof(int), &size);
			ptr += memcpy_send(ptr, GAME_INTRO_MSG, len, &size);
			ptr = buf;

			// ������ ũ�� ��ŷ
			memcpy(ptr, &size, sizeof(int));

			// ���� �۽�
			retval = send(sock, buf, size + sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send");
				return;
			}

			// �۽��� ������ �������� ����
			protocol = RUN;
			continue;

		case RUN:
			len = strlen(GAME_INFO_MSG);

			// �޽��� ���� ��ŷ
			ptr += memcpy_send(ptr, &len, sizeof(int), &size);
			ptr += memcpy_send(ptr, GAME_INFO_MSG, len, &size);
			ptr = buf;

			// ������ ũ�� ��ŷ
			memcpy(ptr, &size, sizeof(int));

			// ���� �۽�
			retval = send(sock, buf, size + sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send");
				return;
			}

			// ������ ũ�� ����
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

			switch (answer_protocol)
			{
			case ANSWER:
				// �޽��� ���� ����ŷ ( Ŭ���̾�Ʈ�� �Է��� �� )
				ptr += memcpy_recv(&len, ptr, sizeof(int));
				ptr += memcpy_recv(&msg, ptr, len);

				// ���� ���࿡ �ʿ��� ����
				int strike = 0;
				int ball = 0;
				int out = 0;

				bool nohit;
				bool error_overrap = false;;
				bool error_overhead = false;

				// �Է��� ���� 3 �ڸ� �̻��� �� ���� üũ
				if (len > 3) {
					error_overhead = true;
				}

				if (!error_overhead || !error_overrap) {

					// �޽����� ���� �Է°��� ���������� ĳ���� ��, �Է� ���� ���� ������ �迭�� ����
					for (int i = 0; i < 3; i++) {
						answer[i] = (int)msg[i] - 48;
					}

					// �Է��� ���� �ߺ��� ���� �ִ� ��, ���� 3 �ڸ� ������ �� üũ
					for (int i = 0; i < 3; i++) {
						if (answer[i] > 10 || answer[i] < 1) {
							error_overhead = true;
							break;
						}

						for (int j = 0; j < i; j++) {
							if (answer[i] == answer[j]) {
								error_overrap = true;
							}
						}
					}

					// ��Ʈ����ũ, ��, �ƿ� üũ
					for (int i = 0; i < 3; i++) {
						nohit = false;
						for (int j = 0; j < 3; j++) {
							if (number[i] == answer[j]) {
								nohit = true;
								if (i == j) {
									strike++;
								}
								else {
									ball++;
								}
							}
						}
						if (!nohit) {
							out++;
						}
					}
				}


				// �ߺ� ���� �޽���
				if (error_overrap) {
					strcpy(msg, GAME_OVERRAP_MSG);
				}

				// 3 �ڸ� ������ �� ���� �޼���
				else if (error_overhead) {
					strcpy(msg, GAME_OVERHEAD_MSG);
				}

				// ������ ���� ��, �������� �޼���
				else {
					sprintf(msg, "��Ʈ����ũ : %d , �� : %d  , �ƿ� : %d\n", strike, ball, out);
					index++;
				}

				size = 0;
				ptr = buf + sizeof(int);
				len = strlen(msg);

				// ��������, �޼��� ���� ��ŷ
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

				// ��Ʈ����ũ�� 3�� �� ��쿡 �������� ����
				if (strike == 3) {
					protocol = END;
				}
				break;
			}

			continue;

		case END:
			sprintf(msg, "%d ������ ���ӿ��� �¸��ϼ̽��ϴ�.\n", index);
			len = strlen(msg);

			// �޼��� ���� ��ŷ
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

			// �ְ�,���� ��� ���� �۾�
			for (int i = 0; i < count; i++) {
				if (!strcmp(userlist[i].id, clientuser.id) && !strcmp(userlist[i].pw, clientuser.pw)) {
					if (userlist[i].high_score == 0) {
						userlist[i].high_score = index;
					}
					else {
						userlist[i].high_score = min(userlist[i].high_score, index);
					}

					if (userlist[i].low_score == 0) {
						userlist[i].low_score = index;
					}
					else {
						userlist[i].low_score = max(userlist[i].low_score, index);
					}
				}
			}

			// �ְ�,���� ����� �ܺ� ���Ͽ� ����
			FileSave(userlist, count);
			return;
		}
	}
}

// ���� ���� �Լ�
bool FileSave(User* user, int count) {
	FILE* fp = fopen("ServerData.txt", "w");		// ������ �� ������ ����

	// ���� üũ
	if (fp == nullptr) {
		fclose(fp);
		return false;
	}

	if (fp) {
		// ���� ������ ������ �ϳ��� ���ٸ� �ٷ� �Լ� ����
		if (count == 0) {
			fclose(fp);
			return false;
		}
		// ���� ������ ������ ����
		fprintf(fp, "%d\n", count);
		// ���������� ������ ������ ����
		for (int i = 0; i < count; i++) {
			fprintf(fp, "%s\n%s\n%d\n%d\n", user[i].id, user[i].pw, user[i].high_score, user[i].low_score);
		}
	}

	//printf("\n[ Save Complete ]\n");
	fclose(fp);	
	return true;
}

// ���� �б� �Լ�
bool FileLoad(User* user, int *count) {
	FILE* fp = fopen("ServerData.txt", "r");		// ������ ���� ������ ����

	// ���� üũ
	if (fp == nullptr) {
		fclose(fp);
		return false;
	}

	if (!feof(fp)) {
		// ���Ͽ� ����� ���� ������ ������ ���� �о�´�
		fscanf_s(fp, "%d\n", *&count);

		// ���������� ���Ͽ� ����� ������ ������ ������ �迭�� �о�´�.
		for (int i = 0; i < *count; i++) {
			fgets(user[i].id, 20, fp);
			user[i].id[strlen(user[i].id) - 1] = '\0';
			fgets(user[i].pw, 20, fp);
			user[i].pw[strlen(user[i].pw) - 1] = '\0';
			fscanf(fp, "%d\n", &user[i].high_score);
			fscanf(fp, "%d\n", &user[i].low_score);
		}
	}

	printf("\n[ Load Complete ]\n");
	fclose(fp);
	return true;
}