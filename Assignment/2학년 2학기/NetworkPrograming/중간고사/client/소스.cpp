#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#include <conio.h>

#define SERVERIP   "211.109.106.161"	// 아이피
#define SERVERPORT 9000		// 포트 번호
#define BUFSIZE 4092				// 버퍼 사이즈
#define MSGSIZE 512					// 문자열 사이즈

// 메뉴 메세지
#define MAINMENU_MSG "\n< 메뉴 >\n1.회원가입\n2.로그인\n3.종료\n\n선택 : "
#define LOGINMENU_MSG "\n< 숫자 야구 게임 >\n1.게임시작\n2.회원탈퇴\n3.종료\n\n선택 : "

#define ANSWER 1						// ANSWER 프로토콜

// 프로토콜 열거형
enum RESULT { ID_MISSING, ID_OVERRAP, PW_MISSING, LOG_NEW, LOGIN_RESULT };
enum PROTOCOL { INTRO = 1, RUN, END };
enum LOGIN_PROTOCOL { NEW = 1, LOGIN , EXIT };
enum GAME_PROTOCOL { START = 1, LOGDEL , LOGOUT };

// 유저 정보를 저장하는 구조체
struct User {
	char id[20];
	char pw[20];

	int high_score;
	int low_score;
};

// 전역 함수 선언
void MainMenu(SOCKET);
void LoginMenu(SOCKET);
void NumBaseBall(SOCKET);

// 소켓 함수 오류 출력 후 종료
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

// 소켓 함수 오류 출력
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

// 사용자 정의 데이터 수신 함수
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

// 소켓 초기화 함수
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

// 가변 길이 패킹 함수들은 내부에서 버퍼 사이즈 증가와 메모리 복사를 처리하고, 복사하는 데이터의 사이즈를
// 리턴해서 주소값을 증가시키는 원리로 설계하였음

// 가변 길이 패킹 함수
int memcpy_send(void* ptr, const void* dst, size_t size, int* len) {
	memcpy(ptr, dst, size);		// 메모리 복사
	*len += size;							// 메모리의 크기를 인자값으로 받은 크기만큼 증가
	return size;							// 인자값으로 받은 크기 리턴
}

// 가변 길이 언패킹 함수
int memcpy_recv(void* dst, const void* ptr, size_t size) {
	memcpy(dst, ptr, size);		// 메모리 복사
	return size;							// 인자값으로 받은 크기 리턴
}

// 메인 함수
int main() {
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 클라이언트 소켓 초기화
	SOCKET sock = sock_init();

	// 메인 메뉴 함수 실행
	MainMenu(sock);

	// closesocket()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}

// 메인 메뉴 함수
void MainMenu(SOCKET sock) {
	int retval;

	User user;	// 유저 정보를 저장할 변수
	ZeroMemory(&user, sizeof(User));	// 초기화 작업

	char buf[BUFSIZE];
	char msg[MSGSIZE];

	while (1) {
		int size = 0;
		char* ptr = buf;
		int len = 0;

		PROTOCOL protocol;
		LOGIN_PROTOCOL sel;
		RESULT result;

		// 메뉴 출력
		printf("%s", MAINMENU_MSG);

		// 메뉴에서 선택할 값 입력
		scanf("%s", msg);

		// 1 자리 이외의 자리는 필요 없으므로 공백 처리
		msg[1] = '\0';

		// 입력한 문자열을 프로토콜로 캐스팅 후, 프로토콜 변수에 저장
		sel = (LOGIN_PROTOCOL)atoi(msg);

		// 프로토콜 정보 송신
		retval = send(sock, (char*)&sel, sizeof(LOGIN_PROTOCOL), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send");
			break;
		}

		// 프로토콜 정보에 따른 작업
		switch (sel) {
		case NEW:
		case LOGIN:
			// 수신 받을 버퍼의 사이즈 수신
			retval = recvn(sock, (char*)&size, sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("recvn");
				return;
			}
			else if (retval == 0) {
				return;
			}

			// 버퍼 수신
			retval = recvn(sock, buf, size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recvn");
				return;
			}
			else if (retval == 0) {
				return;
			}

			// 프로토콜 정보 언패킹
			ptr += memcpy_recv(&protocol, ptr, sizeof(PROTOCOL));

			// 프로토콜 정보에 따른 작업
			switch (protocol) {
			case INTRO:
				// 메세지 정보 언패킹
				ptr += memcpy_recv(&len, ptr, sizeof(int));
				ptr += memcpy_recv(&msg, ptr, len);

				// 언패킹한 메세지 정보 출력
				msg[len] = '\0';
				printf("%s", msg);
				break;
			}

			// 유저의 ID, PW 정보 입력
			printf("\nID : ");
			scanf("%s", user.id);
			printf("\nPASSWORD : ");
			scanf("%s", user.pw);
			break;

		case EXIT:
			// 함수 종료
			return;

		default:
			// 프로토콜 범위 밖이면 처음으로
			printf("\n\n잘못된 입력입니다.\n");
			if (getch()) {
				system("cls");
				continue;
			}
			break;
		}

		ptr = buf + sizeof(int);
		len = sizeof(User);

		// 프로토콜, 유저 정보를 패킹
		ptr += memcpy_send(ptr, &sel, sizeof(LOGIN_PROTOCOL), &size);
		ptr += memcpy_send(ptr, &len, sizeof(int), &size);
		ptr += memcpy_send(ptr, (char*)&user, len, &size);
		ptr = buf;

		// 버퍼의 크기를 패킹
		memcpy(ptr, &size, sizeof(int));

		// 버퍼 송신
		retval = send(sock, buf, size + sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send");
			break;;
		}

		// 수신할 버퍼의 크기를 수신
		retval = recvn(sock, (char*)&size, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("recvn");
			return;
		}
		else if (retval == 0) {
			return;
		}

		// 버퍼 수신
		retval = recvn(sock, buf, size, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recvn");
			return;
		}
		else if (retval == 0) {
			return;
		}

		// 프로토콜, 메시지 정보를 언패킹
		ptr += memcpy_recv(&result, ptr, sizeof(RESULT));
		ptr += memcpy_recv(&len, ptr, sizeof(int));
		ptr += memcpy_recv(&msg, ptr, len);

		// 언패킹한 메시지의 정보를 출력
		msg[len] = '\0';
		printf("%s", msg);

		// 프로토콜에 따른 작업
		switch (result) {
		case LOGIN_RESULT:
			// 로그인 메뉴 함수 실행
			if (getch()) {
				system("cls");
				LoginMenu(sock);
			}
			break;
		default:
			// 로그인 실패이기 때문에 처음으로
			if (getch()) {
				system("cls");
			}
			break;
		}
	}
}

// 로그인 메뉴 함수
void LoginMenu(SOCKET sock) {
	int retval;

	char buf[BUFSIZE];
	char msg[MSGSIZE];

	while (1) {
		int size = 0;
		char* ptr = buf;
		int len = 0;

		GAME_PROTOCOL sel;

		// 로그인 메뉴 출력
		printf("%s", LOGINMENU_MSG);
		
		// 선택한 메뉴값 입력
		scanf("%s", msg);

		// 1 자리 이외에는 필요 없으므로, 공백 처리
		msg[1] = '\0';
		
		// 입력한 문자열을 프로토콜로 캐스팅 이후, 프로토콜 변수에 저장
		sel = (GAME_PROTOCOL)atoi(msg);

		// 프로토콜 정보 송신
		retval = send(sock, (char*)&sel, sizeof(GAME_PROTOCOL), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send");
			break;
		}

		// 프로토콜 정보에 따른 작업
		switch (sel) {
		case START:
			// 숫자 야구 게임 함수 실행
			NumBaseBall(sock);
			break;

		case LOGDEL:
		case LOGOUT:
			// 수신할 버퍼의 크기를 수신
			retval = recvn(sock, (char*)&size, sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("recvn");
				return;
			}
			else if (retval == 0) {
				return;
			}

			// 버퍼 수신
			retval = recvn(sock, buf, size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recvn");
				return;
			}
			else if (retval == 0) {
				return;
			}

			// 포로토콜, 메세지 정보 언패킹
			ptr += memcpy_recv(&sel, ptr, sizeof(GAME_PROTOCOL));
			ptr += memcpy_recv(&len, ptr, sizeof(int));
			ptr += memcpy_recv(&msg, ptr, len);

			// 언패킹한 메시지 정보를 출력
			msg[len] = '\0';
			printf("%s", msg);

			if (getch()) {
				system("cls");
			}
			return;
		}
	}
}

// 숫자 야구 게임 함수
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

		// 수신할 버퍼의 크기를 수신
		retval = recvn(sock, (char*)&size, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("recvn");
			return;
		}
		else if (retval == 0) {
			return;
		}

		// 버퍼 수신
		retval = recvn(sock, buf, size, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recvn");
			return;
		}
		else if (retval == 0) {
			return;
		}

		// 프로토콜 정보 언패킹
		ptr += memcpy_recv(&protocol, ptr, sizeof(PROTOCOL));

		// 프로토콜 정보에 따른 작업
		switch (protocol) {
		case INTRO:
			// 메세지 정보 언패킹
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

			// 언패킹한 메시지 정보를 출력
			msg[len] = '\0';
			printf("%s", msg);
			continue;

		case RUN:
			//메세지 정보 언패킹
			ptr += memcpy_recv(&len, ptr, sizeof(int));
			ptr += memcpy_recv(&msg, ptr, len);

			// 언패킹한 메시지 정보를 출력
			msg[len] = '\0';
			printf("%s", msg);

			// 정답을 입력
			scanf("%s", msg);

			size = 0;
			ptr = buf + sizeof(int);
			len = strlen(msg);

			// 프로토콜, 메시지 정보 패킹
			ptr += memcpy_send(ptr, &answer_protocol, sizeof(ANSWER), &size);
			ptr += memcpy_send(ptr, &len, sizeof(int), &size);
			ptr += memcpy_send(ptr, &msg, len, &size);
			ptr = buf;

			// 버퍼의 크기 패킹
			memcpy(ptr, &size, sizeof(int));

			// 버퍼 송신
			retval = send(sock, buf, size + sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send");
				return;
			}

			// 수신할 버퍼의 크기를 수신
			retval = recvn(sock, (char*)&size, sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("recvn");
				return;
			}
			else if (retval == 0) {
				return;
			}

			// 버퍼 수신
			retval = recvn(sock, buf, size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recvn");
				return;
			}
			else if (retval == 0) {
				return;
			}

			// 프로토콜 정보 언패킹
			ptr += memcpy_recv(&answer_protocol, ptr, sizeof(ANSWER));

			// 프로토콜 정보에 따른 작업
			switch (answer_protocol) {
			case ANSWER:
				// 메시지 정보 언패킹
				ptr += memcpy_recv(&len, ptr, sizeof(int));
				ptr += memcpy_recv(&msg, ptr, len);

				// 언패킹한 메시지 정보를 출력
				msg[len] = '\0';
				printf("%s", msg);
				break;
			}
			continue;

		case END:
			// 메세지 정보 언패킹
			ptr += memcpy_recv(&len, ptr, sizeof(int));
			ptr += memcpy_recv(&msg, ptr, len);

			// 언패킹한 메시지 정보를 출력
			msg[len] = '\0';
			printf("\n%s", msg);
			
			if (getch()) {
				system("cls");
			}
			return;
		}
	}
}
