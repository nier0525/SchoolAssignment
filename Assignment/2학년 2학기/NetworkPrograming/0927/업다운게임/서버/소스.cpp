#pragma comment (lib,"ws2_32")
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <WinSock2.h>

#define SERVERPORT 9000
#define BUFSIZE 512
#define MAX 100

#define ID "\nID : "
#define PASS "PASS : "

#define max(a,b) ((a > b) ? b : a);

// 유저 구조체
struct User {
	char id[20];
	char pass[20];
	int score;
};

bool FileSave(User*, int*);
bool FileLoad(User*, int*);

void Err_Quit(const char*);
void Err_DisPlay(const char*);
void LoginMenu(SOCKET, User*, User*, int*);
int UpDownGame(SOCKET, User*, User*, int*);

// 소켓 초기화
SOCKET SockInit() {
	int retval = 0;

	SOCKET Listen_Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (Listen_Sock == INVALID_SOCKET) {
		Err_Quit("socket()");
	}

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);

	retval = bind(Listen_Sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	if (retval == SOCKET_ERROR) {
		Err_Quit("Bind()");
	}

	retval = listen(Listen_Sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) {
		Err_Quit("Listen()");
	}

	return Listen_Sock;
}

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

// 넘겨 받는 데이터의 용량을 정확히 파악하고 있을 때
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

// 로그인 시스템 함수
void LoginSystem(SOCKET sock, User* user, int* count) {
	char log_buf[BUFSIZE];
	char temp[BUFSIZE];

	int retval = 0;
	int num = 0;
	int userlen = 0;

	int GetSize = 0;
	char usertemp[BUFSIZE];
	User* clientuser;
	ZeroMemory(&clientuser, sizeof(clientuser));

	char menu_buf[BUFSIZE];
	strcpy(menu_buf, "[ 로그인 시스템 ]\n\n1.로그인\n2.회원가입\n3.종료\n\n선택 : ");

	while (1) {
		bool flug = false;
		
		retval = send(sock, menu_buf, strlen(menu_buf), 0);
		if (retval == SOCKET_ERROR) {
			Err_DisPlay("send_MenuSelect");
			break;
		}

		retval = recv(sock, temp, BUFSIZE, 0);
		if (retval == SOCKET_ERROR) {
			Err_DisPlay("recv_MenuSelect");
			break;
		}

		temp[retval] = '\0';
		num = atoi(temp);

		if (num == 1 || num == 2) {
			retval = recvn(sock, (char*)&userlen, sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				Err_DisPlay("recvn_log");
				break;
			}

			retval = send(sock, (char*)ID, strlen(ID), 0);
			if (retval == SOCKET_ERROR) {
				Err_DisPlay("send_log");
				break;
			}
			retval = send(sock, (char*)PASS, strlen(PASS), 0);
			if (retval == SOCKET_ERROR) {
				Err_DisPlay("send_log");
				break;
			}

			GetSize = recv(sock, usertemp, userlen, 0);
			if (GetSize == SOCKET_ERROR) {
				Err_DisPlay("recv_log");
				break;
			}

			usertemp[GetSize] = '\0';
			clientuser = (User*)usertemp;
		}

		switch (num) {

		case 1:
			for (int i = 0; i < *count; i++) {
				if (!strcmp(user[i].id, clientuser->id) && !strcmp(user[i].pass, clientuser->pass)) {
					strcpy(log_buf, "\n로그인 성공\n");
					retval = send(sock, log_buf, strlen(log_buf), 0);
					if (retval == SOCKET_ERROR) {
						Err_DisPlay("send_log");
						return;
					}
					printf("\n[TCP 클라이언트] %s 로그인\n", clientuser->id);
					flug = true;

					LoginMenu(sock, user, clientuser, count);
					break;
				}
			}

			if (!flug) {
				strcpy(log_buf, "\nID 또는 PASSWORD 불일치\n");
				retval = send(sock, log_buf, strlen(log_buf), 0);
				if (retval == SOCKET_ERROR) {
					Err_DisPlay("send_log");
					return;
				}
			}
			break;

		case 2:
			for (int i = 0; i < *count; i++) {
				if (!strcmp(user[i].id, clientuser->id)) {
					strcpy(log_buf, "\n아이디 중복 !\n");
					retval = send(sock, log_buf, strlen(log_buf), 0);
					if (retval == SOCKET_ERROR) {
						Err_DisPlay("send_log");
						return;
					}
					flug = true;
					break;
				}
			}

			if (!flug) {
				strcpy(log_buf, "\n회원가입 완료 !\n");
				retval = send(sock, log_buf, strlen(log_buf), 0);
				if (retval == SOCKET_ERROR) {
					Err_DisPlay("send_log");
					return;
				}

				user[*count] = *clientuser;
				*count += 1;

				printf("\n[TCP 클라이언트] %s 회원가입 완료\n", clientuser->id);
			}
			break;

		case 3:
			return;
			break;

		default:
			continue;
			break;
		}
	}
}

// 로그인 이후 메뉴 함수
void LoginMenu(SOCKET sock, User* user, User* clientuser, int* count) {
	char log_buf[BUFSIZE];
	char temp[BUFSIZE];

	int retval = 0;
	int num = 0;

	char menu_buf[BUFSIZE];
	strcpy(menu_buf, "[ 로그인 시스템 ]\n\n1.게임시작\n2.회원탈퇴\n3.로그아웃\n\n선택 : ");

	while (1) {
		retval = send(sock, menu_buf, strlen(menu_buf), 0);
		if (retval == SOCKET_ERROR) {
			Err_DisPlay("send_MenuSelect");
			break;
		}

		retval = recv(sock, temp, BUFSIZE, 0);
		if (retval == SOCKET_ERROR) {
			Err_DisPlay("recv_MenuSelect");
			break;
		}

		temp[retval] = '\0';
		num = (int)temp[0] - 48;

		switch (num) {
		case 1:
			for (int i = 0; i < *count; i++) {
				if (!strcmp(user[i].id, clientuser->id)) {
					user[i].score = UpDownGame(sock, user, clientuser, count);
					break;
				}
			}
			break;

		case 2:
			for (int i = 0; i < *count; i++) {
				if (!strcmp(user[i].id, clientuser->id)) {
					for (int j = i; j < *count - 1; j++) {
						user[j] = user[j + 1];
					}
					ZeroMemory(&user[*count - 1], sizeof(user));
					break;
				}
			}

			strcpy(log_buf, "\n탈퇴 완료\n");

			retval = send(sock, log_buf, strlen(log_buf), 0);
			if (retval == SOCKET_ERROR) {
				Err_DisPlay("send_Delete");
				return;
			}

			printf("\n[TCP 클라이언트] %s 탈퇴\n", clientuser->id);
			*count -= 1;
			
			return;
			break;

		case 3:
			return;
			break;

		default:
			continue;
			break;
		}
	}
}

// UpDown 게임 실행 함수
int UpDownGame(SOCKET sock, User* user, User* clinetuser, int* count) {
	char log_buf[BUFSIZE];

	char buf[BUFSIZE];
	
	char temp[BUFSIZE];
	char index[BUFSIZE];

	int retval = 0;

	int score = 0;
	int cur = 0;
	int num = 0;

	srand((unsigned)time(NULL));
	cur = rand() % 100 + 1;

	for (int i = 0; i < *count; i++) {
		if (!strcmp(user[i].id, clinetuser->id)) {
			strcpy(temp, user[i].id);

			strcat(temp, " 님의 최고 점수 : ");
			sprintf(index, "%d\n", user[i].score);
			
			strcat(temp, index);
			strcat(temp, "\n1~100 까지 숫자 중 정답이 있습니다.\n\n");

			retval = send(sock, temp, strlen(temp), 0);
			if (retval == SOCKET_ERROR) {
				Err_DisPlay("send_score");
				return user[i].score;
			}

			while (1) {
				score++;

				retval = recv(sock, temp, strlen(temp), 0);
				if (retval == SOCKET_ERROR) {
					Err_DisPlay("recv_game");
					return user[i].score;
				}

				temp[retval] = '\0';

				retval = send(sock, temp, strlen(temp), 0);
				if (retval == SOCKET_ERROR) {
					Err_DisPlay("send_game");
					return user[i].score;
				}

				retval = recv(sock, index, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					Err_DisPlay("recv_game");
					return user[i].score;
				}

				index[retval] = '\0';

				num = atoi(index);

				if (num == cur) {
					sprintf(index, "%d", score);
					strcpy(temp, " 번만에 맞추셨습니다.\n");
					strcat(index, temp);

					retval = send(sock, index, strlen(index), 0);
					if (retval == SOCKET_ERROR) {
						Err_DisPlay("send_game");
						return user[i].score;
					}

					if (user[i].score == 0) {
						return score;
					}
					else {
						return max(user[i].score, score);
					}
				}

				else {
					if (num < cur) {
						strcpy(index, "\nUp\n");

						retval = send(sock, index, strlen(index), 0);
						if (retval == SOCKET_ERROR) {
							Err_DisPlay("send_game");
							return user[i].score;
						}
					}
					if (num > cur) {
						strcpy(index, "\nDown\n");

						retval = send(sock, index, strlen(index), 0);
						if (retval == SOCKET_ERROR) {
							Err_DisPlay("send_game");
							return user[i].score;
						}
					}
				}
			}

			break;
		}
	}
}

int main() {

	int retval;

	// 유저 리스트
	User user[MAX];
	ZeroMemory(&user, sizeof(user));
	
	// 유저수
	int count = 0;

	// 데이터 불러오기 ( 유저 정보, 유저 수 )
	FileLoad(user, &count);

	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		Err_Quit("WSAStartup()");
	}

	// 소켓 초기화 함수
	SOCKET Listen_Sock = SockInit();

	printf("\n[TCP/IP 로그인 시스템 (UP Down Game)]\n");
	printf("\n현재 등록된 계정 : %d 개\n", count);

	SOCKET Client;
	SOCKADDR_IN ClientServer;
	
	int len;
	int count_temp = 0;
	User user_temp[MAX];
	ZeroMemory(&user_temp, sizeof(user_temp));

	while (1) {
		len = sizeof(ClientServer);
		Client = accept(Listen_Sock, (SOCKADDR*)&ClientServer, &len);
		if (Client == INVALID_SOCKET) {
			Err_DisPlay("Accept()");
		}

		// 기존의 유저의 데이터 저장
		count_temp = count;
		for (int i = 0; i < count_temp; i++) {
			user_temp[i] = user[i];
		}

		printf("\n현재 등록된 계정 : %d 개\n", count);

		printf("\n[TCP/IP 클라이언트 접속] IP : %s , PORT : %d\n", inet_ntoa(ClientServer.sin_addr),
			ntohs(ClientServer.sin_port));

		// 로그인 프로그램 실행
		LoginSystem(Client, user, &count);

		closesocket(Client);
		printf("\n[TCP 클라이언트 접속 종료] IP : %s , PORT : %d\n", inet_ntoa(ClientServer.sin_addr), ntohs(ClientServer.sin_port));

		FileSave(user, &count);
	}

	closesocket(Listen_Sock);
	WSACleanup();
	return 0;
}


// 데이터 저장하기
bool FileSave(User* user, int* count) {
	FILE* fp = fopen("ServerData.txt", "wb");

	if (fp == nullptr) {
		fclose(fp);
		return false;
	}

	if (fp) {
		if (*count == 0) {
			fclose(fp);
			return false;
		}
		fprintf(fp, "%d\n", *count);
		for (int i = 0; i < *count; i++) {
			fprintf(fp, "%s\n%s\n%d\n", user[i].id, user[i].pass, user[i].score);
		}
	}

	/*MessageBox(NULL, "Save Complete", "ServerData", MB_OK);*/
	printf("\n[ Save Complete ]\n");
	fclose(fp);
	return true;
}

// 데이터 불러오기
bool FileLoad(User* user, int *count) {
	FILE* fp = fopen("ServerData.txt", "rb");

	if (fp == nullptr) {
		fclose(fp);
		return false;
	}

	if (!feof(fp)) {
		fscanf_s(fp, "%d\n", *&count);

		for (int i = 0; i < *count; i++) {
			//fscanf_s(fp, "%s,%s,%d\n", user[i].id, user[i].pass, &user[i].score);
			fgets(user[i].id, 20, fp);
			user[i].id[strlen(user[i].id) - 1] = '\0';
			fgets(user[i].pass, 20, fp);
			user[i].pass[strlen(user[i].pass) - 1] = '\0';
			fscanf(fp, "%d\n", &user[i].score);
		}
	}

	printf("\n[ Load Complete ]\n");
	fclose(fp);
	return true;
}
