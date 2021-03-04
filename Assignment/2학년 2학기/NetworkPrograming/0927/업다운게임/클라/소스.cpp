#pragma comment (lib,"ws2_32")
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <conio.h>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512

// 유저 구조체
struct User {
	char id[20];
	char pass[20];
	int score;
};

void LoginMenu(SOCKET, User);

// 에러 체크 ( 종료 )
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

// 에러 체크 ( 출력 )
void Err_DisPlay(const char* msg) {
	LPVOID IpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&IpMsgBuf, 0, NULL);

	printf("[%s] %s\n", msg, (char*)IpMsgBuf);
	LocalFree(IpMsgBuf);
}

// 소켓 초기화
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

// 받을 데이터의 용량을 정확히 알고 있을 때
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

// 메인 메뉴
void MainMenu(SOCKET sock, User user) {
	int retval;

	char buf[BUFSIZE + 1];

	char log[BUFSIZE];

	int userlen;

	while (1) {
		retval = recv(sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR) {
			Err_DisPlay("recv_MenuSelect");
			break;
		}

		buf[retval] = '\0';
		printf("%s", buf);

		if (fgets(buf, BUFSIZE, stdin) == nullptr) {
			break;
		}

		retval = send(sock, buf, strlen(buf), 0);
		if (retval == SOCKET_ERROR) {
			Err_DisPlay("send_MenuSelect");
			break;
		}

		buf[strlen(buf) - 1] = '\0';

		if (!strcmp(buf, "3")) {
			break;
		}

		else if (!strcmp(buf, "1") || !strcmp(buf, "2")) {
			userlen = sizeof(user);
			retval = send(sock, (char*)&userlen, sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				Err_DisPlay("send_log");
				break;
			}

			retval = recv(sock, log, BUFSIZE, 0);
			if (retval == SOCKET_ERROR) {
				Err_DisPlay("recv_log");
				break;
			}
			log[retval] = '\0';

			printf("%s", log);

			if (fgets(user.id, 20, stdin) == nullptr) {
				break;
			}

			retval = recv(sock, log, BUFSIZE, 0);
			if (retval == SOCKET_ERROR) {
				Err_DisPlay("recv_log");
				break;
			}
			log[retval] = '\0';

			printf("%s", log);

			if (fgets(user.pass, 20, stdin) == nullptr) {
				break;
			}

			user.id[strlen(user.id) - 1] = '\0';
			user.pass[strlen(user.pass) - 1] = '\0';

			retval = send(sock, (char*)&user, sizeof(User), 0);
			if (retval == SOCKET_ERROR) {
				Err_DisPlay("send_log");
				break;
			}
		}

		else {
			fflush(stdin);
			system("cls");
			continue;
		}

		retval = recv(sock, log, BUFSIZE, 0);
		if (retval == SOCKET_ERROR) {
			Err_DisPlay("recv_log");
			break;
		}
		log[retval] = '\0';

		printf("%s", log);

		if (!strcmp(log, "\n로그인 성공\n")) {
			if (getch()) {
				system("cls");
			}
			LoginMenu(sock, user);
		}

		else {
			if (getch()) {
				system("cls");
				continue;
			}
		}
	}
}

// 로그인 메뉴
void LoginMenu(SOCKET sock, User user) {
	int retval;

	char buf[BUFSIZE + 1];
	char log[BUFSIZE];

	while (1) {
		retval = recv(sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR) {
			Err_DisPlay("recv_LoginMenu");
			break;
		}

		buf[retval] = '\0';
		printf("%s", buf);

		if (fgets(buf, BUFSIZE, stdin) == nullptr) {
			break;
		}

		retval = send(sock, buf, strlen(buf), 0);
		if (retval == SOCKET_ERROR) {
			Err_DisPlay("send_LoginMenu");
			break;
		}

		buf[strlen(buf) - 1] = '\0';

		if (!strcmp(buf, "3")) {
			system("cls");
			break;
		}

		else if (!strcmp(buf, "1")) {

			system("cls");

			retval = recv(sock, log, BUFSIZE, 0);
			if (retval == SOCKET_ERROR) {
				Err_DisPlay("recv_score");
				break;
			}

			log[retval] = '\0';
			printf("%s", log);

			while (1) {
				strcpy(log, "입력 : ");

				retval = send(sock, log, strlen(log), 0);
				if (retval == SOCKET_ERROR) {
					Err_DisPlay("send_game");
					break;
				}

				retval = recv(sock, log, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					Err_DisPlay("recv_game");
					break;
				}

				log[retval] = '\0';
				printf("%s", log);

				if (fgets(log, BUFSIZE, stdin) == NULL) {
					break;
				}

				retval = send(sock, log, strlen(log), 0);
				if (retval == SOCKET_ERROR) {
					Err_DisPlay("send_game");
					break;
				}

				retval = recv(sock, log, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					Err_DisPlay("recv_score");
					break;
				}

				log[retval] = '\0';

				printf("%s", log);

				if (strcmp(log, "\nUp\n") && strcmp(log, "\nDown\n")) {
					if (getch()) {
						system("cls");
						break;
					}
				}
			}
			continue;
		}

		else if (!strcmp(buf, "2")) {
			retval = recv(sock, log, BUFSIZE, 0);
			if (retval == SOCKET_ERROR) {
				Err_DisPlay("recv_Delete");
				return;
			}

			log[retval] = '\0';
			printf("%s", log);

			if (getch()) {
				system("cls");
			}
			break;
		}

		else {
			fflush(stdin);
			system("cls");
			continue;
		}
	}
}

int main() {

	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		Err_Quit("WSAStartup()");
	}

	SOCKET sock = SockInit();

	User user;
	ZeroMemory(&user, sizeof(user));

	MainMenu(sock, user);

	printf("\n\n프로그램 종료 . . .\n");

	closesocket(sock);
	WSACleanup();
	return 0;
}