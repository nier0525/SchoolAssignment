#pragma comment (lib, "ws2_32")
#include <WinSock2.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 최고, 최저 점수를 추려내기 위한 함수 매크로
#define max(a,b) ((a  > b) ? a : b)
#define min(a,b) ((a < b) ? a : b)

#define SERVERPORT 9000	// 포트 번호
#define BUFSIZE 4092			// 버퍼 사이즈
#define MSGSIZE 512				// 문자열 사이즈
#define USERSIZE 100			// 유저 정보를 저장할 배열의 크기

#define ANSWER 1					// ANSWER 프로토콜

// 클라이언트로 보낼 메세지
#define LOG_INTRO_MSG "\n\nID 와 PASSWORD 를 입력하세요\n"
#define GAME_INTRO_MSG "\n숫자 야구 게임을 시작합니다. 범위는 1~9 까지 입니다.\n"

#define GAME_INFO_MSG "\n3 개의 숫자를 입력하세요. ( 예 : 456 , 174 등등 )\n입력 : "
#define GAME_OVERRAP_MSG "\n입력된 값 중 중복된 값이 있으면 안됩니다.\n"
#define GAME_OVERHEAD_MSG "\n입력된 값이 잘못된 형식입니다.\n"
#define GAME_END_MSG "\n\n게임에서 승리하셨습니다!\n"

#define ID_MISSING_MSG "\n없는  ID 입니다.\n"
#define ID_OVERRAP_MSG "\n중복된 ID 입니다.\n"
#define PW_MISSING_MSG "\nPASSWORD 가 틀렸습니다.\n"

#define NEW_MSG "\n정상적으로 회원가입 되었습니다.\n"
#define LOGIN_MSG "\n정상적으로 로그인 되었습니다.\n"
#define LOGOUT_MSG "\n정상적으로 로그아웃 되었습니다.\n"
#define DELETE_MSG "\n정상적으로 회원탈퇴 되었습니다.\n"

// 프로토콜 열거형
enum RESULT { ID_MISSING , ID_OVERRAP, PW_MISSING, LOG_NEW ,LOGIN_RESULT };
enum PROTOCOL { INTRO = 1 , RUN , END };
enum LOGIN_PROTOCOL { NEW = 1, LOGIN, EXIT };
enum GAME_PROTOCOL { START = 1, LOGDEL, LOGOUT };

// 유저 정보를 저장하는 구조체
struct User {
	char id[20];
	char pw[20];

	int high_score;
	int low_score;
};

User userlist[USERSIZE];	// 유저 정보를 저장할 배열
int count = 0;

// 전역 함수 선언
bool FileSave(User*, int);
bool FileLoad(User*, int*);

void MainMenu(SOCKET);
void LoginMenu(SOCKET, User);
void NumBaseBall(SOCKET, User);

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

// 가변 길이 패킹 함수들은 내부에서 버퍼 사이즈 증가와 메모리 복사를 처리하고, 복사하는 데이터의 사이즈를
// 리턴해서 주소값을 증가시키는 원리로 설계하였음

// 가변 길이 패킹 함수
int memcpy_send(void* ptr, const void* dst, size_t size, int* len) {
	memcpy(ptr, dst, size);	// 메모리 복사
	*len += size;						// 메모리의 크기를 인자값으로 받은 크기만큼 증가
	return size;						// 인자값으로 받은 크기 리턴
}

// 가변 길이 언패킹 함수
int memcpy_recv(void* dst, const void* ptr, size_t size) {
	memcpy(dst, ptr, size);	// 메모리 복사
	return size;						// 인자값으로 받은 크기 리턴
}

int main() {
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 서버 소켓 초기화
	SOCKET listen_sock = sock_init();

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;		// 클라이언트 소켓
	SOCKADDR_IN clientaddr;		// 클라이언트 주소 정보
	int addrlen;						// 크기를 저장할 input & output 용으로 쓰일 정수

	// 유저 배열 초기화
	for (int i = 0; i < USERSIZE; i++) {
		ZeroMemory(&userlist[i], sizeof(User));
	}
	// 현재 유저의 계정 개수

	// 시작할 때 외부의 파일에 저장된 유저 정보를 읽어옴
	FileLoad(userlist, &count);

	// rand 의 중복 방지를 위한 srand
	srand(time(nullptr));

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// 접속한 클라이언트 정보 출력
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		printf("\n[TCP 서버] 현재 등록된 계정\n");
		for (int i = 0; i < count; i++) {
			printf("[ Log ] ID : %s , PW : %s\n", userlist[i].id, userlist[i].pw);
		}

		// 메인 메뉴 함수 실행
		MainMenu(client_sock);

		// closesocket()
		closesocket(client_sock);
		printf("\n[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}
	// closesocket()
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}

// 메인 메뉴 함수
void MainMenu(SOCKET sock) {
	int retval;

	char buf[BUFSIZE];
	char msg[MSGSIZE];

	User clientuser;	// 클라이언트 유저 정보를 저장할 변수
	ZeroMemory(&clientuser, sizeof(User));	// 초기화 작업

	while (1) {
		int size = 0;
		char* ptr = buf + sizeof(int);
		PROTOCOL protocol = INTRO;
		LOGIN_PROTOCOL login_protocol;
		RESULT result;
		int len = strlen(LOG_INTRO_MSG);

		// 프로토콜 정보 수신
		retval = recvn(sock, (char*)&login_protocol, sizeof(LOGIN_PROTOCOL), 0);
		if (retval == SOCKET_ERROR) {
			err_display("recvn");
			break;
		}
		else if (retval == 0) {
			break;
		}

		// 프로토콜 정보에 따른 작업
		switch (login_protocol) {
		case NEW:
		case LOGIN:
			// 프로토콜, 메세지 정보 패킹
			ptr += memcpy_send(ptr, &protocol, sizeof(PROTOCOL), &size);
			ptr += memcpy_send(ptr, &len, sizeof(int), &size);
			ptr += memcpy_send(ptr, LOG_INTRO_MSG, len, &size);
			ptr = buf;

			// 버퍼의 크기 패킹
			memcpy(ptr, &size, sizeof(int));

			// 버퍼 송신
			retval = send(sock, buf, size + sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send");
				return;
			}
			break;


		case EXIT:
			// 함수 종료
			return;
			break;

		default:
			// 프로토콜 범위를 초과했을 경우, 처음부터 다시 작업
			continue;
			break;
		}

		// 수신할 버퍼의 크기 수신
		retval = recvn(sock, (char*)&size, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("recvn");
			break;
		}
		else if (retval == 0) {
			break;
		}

		// 버퍼 수신
		retval = recvn(sock, buf, size, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recvn");
			break;
		}
		else if (retval == 0) {
			break;
		}

		// 프로토콜, 클라이언트 유저 정보를 언패킹
		ptr += memcpy_recv(&login_protocol, ptr, sizeof(LOGIN_PROTOCOL));
		ptr += memcpy_recv(&len, ptr, sizeof(int));
		ptr += memcpy_recv(&clientuser, ptr, len);

		// 회원가입, 로그인 작업에 필요한 변수
		bool flug = false;
		bool login = false;

		// 프로토콜 정보에 따른 작업
		switch (login_protocol) {
		case NEW:
			// 유저 정보 갱신 전 ID 중복 체크
			for (int i = 0; i < count; i++) {
				if (!strcmp(userlist[i].id, clientuser.id)) {
					result = ID_OVERRAP;
					strcpy(msg, ID_OVERRAP_MSG);

					flug = true;
					break;
				}
			}

			// ID 중복이 없다면 유저 정보 갱신
			if (!flug) {
				result = LOG_NEW;
				strcpy(msg, NEW_MSG);

				userlist[count] = clientuser;
				printf("[ New Log ] ID : %s , PW : %s\n", userlist[count].id, userlist[count].pw);
				count += 1;

				// 갱신된 유저의 정보 파일에 쓰기
				FileSave(userlist, count);
			}
			break;

		case LOGIN:
			// 클라이언트가 입력한 유저의 정보가 유저 배열에 있는 지 검사
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

			// 아이디가 일치하지 않을 경우 ( 유저 배열에 정보가 없다 )
			if (!flug) {
				result = ID_MISSING;
				strcpy(msg, ID_MISSING_MSG);
			}
			break;
		}

		size = 0;
		ptr = buf + sizeof(int);
		len = strlen(msg);

		// 프로토콜, 메세지 정보 패킹
		ptr += memcpy_send(ptr, &result, sizeof(RESULT), &size);
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

		// 로그인에 성공했다면 로그인 메뉴 함수 실행
		if (login) {
			LoginMenu(sock, clientuser);
		}
	}
}

// 로그인 메뉴 함수
void LoginMenu(SOCKET sock, User clientuser) {
	int retval;

	// 수신,송신에 쓰일 변수
	char buf[BUFSIZE];
	char msg[MSGSIZE];

	while (1) {
		int size = 0;
		char* ptr = buf + sizeof(int);
		GAME_PROTOCOL game_protocol;
		int len = 0;

		// 프로토콜 정보 수신
		retval = recvn(sock, (char*)&game_protocol, sizeof(GAME_PROTOCOL), 0);
		if (retval == SOCKET_ERROR) {
			err_display("recvn");
			break;
		}
		else if (retval == 0) {
			break;
		}

		// 프로토콜 정보에 따른 작업
		switch (game_protocol) {
		case START:
			// 숫자 야구 게임 함수 실행
			NumBaseBall(sock, clientuser);
			continue;

		case LOGDEL:
			// 유저 리스트 배열 내에서 현재의 클라이언트의 유저 정보를 제거 작업
			for (int i = 0; i < count; i++) {
				if (!strcmp(userlist[i].id, clientuser.id) && !strcmp(userlist[i].pw, clientuser.pw)) {
					printf("[ Delete Log ] ID : %s , PW : %s\n", userlist[i].id, userlist[i].pw);
					for (int j = i; j < count - 1; j++) {	// 제거 이후, 빈 자리를 채워넣는 작업
						userlist[j] = userlist[j + 1];
					}
					ZeroMemory(&userlist[count - 1], sizeof(User));
					count -= 1;
				}
			}

			// 메세지와 프로토콜 정보 갱신
			strcpy(msg, DELETE_MSG);
			game_protocol = LOGDEL;

			// 외부에 파일에 삭제 이후의 유저 리스트 배열의 정보를 저장
			FileSave(userlist, count);
			break;

		case LOGOUT:
			// 현재 클라이언트의 유저의 최고,최저의 기록을 출력
			for (int i = 0; i < count; i++) {
				if (!strcmp(userlist[i].id, clientuser.id) && !strcmp(userlist[i].pw, clientuser.pw)) {
					sprintf(msg, "\n현재 유저의 게임 최단 기록은 %d 이고, 최장 기록은 %d 입니다.\n", userlist[i].high_score, userlist[i].low_score);
				}
			}
			// 메세지와 프로토콜 정보 갱신
			strcat(msg, LOGOUT_MSG);
			game_protocol = LOGOUT;
			break;
		}

		len = strlen(msg);

		// 프로토콜, 메세지의 정보를 패킹
		ptr += memcpy_send(ptr, &game_protocol, sizeof(GAME_PROTOCOL), &size);
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
		break;
	}
}

// 숫자 야구 게임 함수
void NumBaseBall(SOCKET sock, User clientuser) {
	int retval;
	int answer_protocol;	// answer 프로토콜

	int number[3];				// 정답을 저장할 배열
	int answer[3];				// 입력 받은 값을 저장할 배열

	int index = 0;				// 게임 진행 횟수

	// 수신,송신에 쓰일 변수
	char buf[BUFSIZE];
	char msg[MSGSIZE];

	PROTOCOL protocol = INTRO;
	int len = strlen(GAME_INTRO_MSG);	

	// 숫자 야구 게임의 정답을 랜덤값으로 초기화 시켜주는 작업
	// for 문을 두 번 도는 것으로 중복되는 값이 없도록 초기화
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
		// 수신, 송신에 쓰일 변수
		int size = 0;
		char* ptr = buf + sizeof(int);

		// 프로토콜 정보 패킹
		ptr += memcpy_send(ptr, &protocol, sizeof(PROTOCOL), &size);

		// 현재 프로토콜 변수에 저장된 프로토콜 값에 의해 작업이 나뉨
		switch (protocol) {
		case INTRO:
			// 메세지 정보 패킹
			sprintf(msg, "\n[ 숫자 야구 랭킹 현황 ] \n--------------------------------------------- \n");
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

			// 버퍼의 크기 패킹
			memcpy(ptr, &size, sizeof(int));

			// 버퍼 송신
			retval = send(sock, buf, size + sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send");
				return;
			}

			// 송신이 끝나면 프로토콜 변경
			protocol = RUN;
			continue;

		case RUN:
			len = strlen(GAME_INFO_MSG);

			// 메시지 정보 패킹
			ptr += memcpy_send(ptr, &len, sizeof(int), &size);
			ptr += memcpy_send(ptr, GAME_INFO_MSG, len, &size);
			ptr = buf;

			// 버퍼의 크기 패킹
			memcpy(ptr, &size, sizeof(int));

			// 버퍼 송신
			retval = send(sock, buf, size + sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send");
				return;
			}

			// 버퍼의 크기 수신
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

			switch (answer_protocol)
			{
			case ANSWER:
				// 메시지 정보 언패킹 ( 클라이언트가 입력한 답 )
				ptr += memcpy_recv(&len, ptr, sizeof(int));
				ptr += memcpy_recv(&msg, ptr, len);

				// 게임 실행에 필요한 변수
				int strike = 0;
				int ball = 0;
				int out = 0;

				bool nohit;
				bool error_overrap = false;;
				bool error_overhead = false;

				// 입력한 값이 3 자리 이상인 지 오류 체크
				if (len > 3) {
					error_overhead = true;
				}

				if (!error_overhead || !error_overrap) {

					// 메시지로 받은 입력값을 정수형으로 캐스팅 후, 입력 받은 값을 저장할 배열에 저장
					for (int i = 0; i < 3; i++) {
						answer[i] = (int)msg[i] - 48;
					}

					// 입력한 값에 중복된 값이 있는 지, 값이 3 자리 이하인 지 체크
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

					// 스트라이크, 볼, 아웃 체크
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


				// 중복 에러 메시지
				if (error_overrap) {
					strcpy(msg, GAME_OVERRAP_MSG);
				}

				// 3 자리 이하의 수 에러 메세지
				else if (error_overhead) {
					strcpy(msg, GAME_OVERHEAD_MSG);
				}

				// 오류가 없을 시, 정상적인 메세지
				else {
					sprintf(msg, "스트라이크 : %d , 볼 : %d  , 아웃 : %d\n", strike, ball, out);
					index++;
				}

				size = 0;
				ptr = buf + sizeof(int);
				len = strlen(msg);

				// 프로토콜, 메세지 정보 패킹
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

				// 스트라이크가 3개 일 경우에 프로토콜 변경
				if (strike == 3) {
					protocol = END;
				}
				break;
			}

			continue;

		case END:
			sprintf(msg, "%d 번만에 게임에서 승리하셨습니다.\n", index);
			len = strlen(msg);

			// 메세지 정보 패킹
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

			// 최고,최저 기록 저장 작업
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

			// 최고,최저 기록을 외부 파일에 저장
			FileSave(userlist, count);
			return;
		}
	}
}

// 파일 쓰기 함수
bool FileSave(User* user, int count) {
	FILE* fp = fopen("ServerData.txt", "w");		// 정보를 쓸 파일의 정보

	// 오류 체크
	if (fp == nullptr) {
		fclose(fp);
		return false;
	}

	if (fp) {
		// 만일 유저의 정보가 하나도 없다면 바로 함수 종료
		if (count == 0) {
			fclose(fp);
			return false;
		}
		// 유저 계정의 개수를 저장
		fprintf(fp, "%d\n", count);
		// 순차적으로 유저의 정보를 저장
		for (int i = 0; i < count; i++) {
			fprintf(fp, "%s\n%s\n%d\n%d\n", user[i].id, user[i].pw, user[i].high_score, user[i].low_score);
		}
	}

	//printf("\n[ Save Complete ]\n");
	fclose(fp);	
	return true;
}

// 파일 읽기 함수
bool FileLoad(User* user, int *count) {
	FILE* fp = fopen("ServerData.txt", "r");		// 정보를 읽을 파일의 정보

	// 오류 체크
	if (fp == nullptr) {
		fclose(fp);
		return false;
	}

	if (!feof(fp)) {
		// 파일에 저장된 유저 계정의 개수를 먼저 읽어온다
		fscanf_s(fp, "%d\n", *&count);

		// 순차적으로 파일에 저장된 유저의 정보를 유저의 배열에 읽어온다.
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