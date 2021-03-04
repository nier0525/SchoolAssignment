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

// ���� ����ü
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

// ���� �ʱ�ȭ
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

// ���� üũ ( ���� )
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

// ���� üũ ( ��� )
void Err_DisPlay(const char* msg) {
	LPVOID IpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&IpMsgBuf, 0, NULL);

	printf("[%s] %s\n", msg, (char*)IpMsgBuf);
	LocalFree(IpMsgBuf);
}

// �Ѱ� �޴� �������� �뷮�� ��Ȯ�� �ľ��ϰ� ���� ��
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

// �α��� �ý��� �Լ�
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
	strcpy(menu_buf, "[ �α��� �ý��� ]\n\n1.�α���\n2.ȸ������\n3.����\n\n���� : ");

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
					strcpy(log_buf, "\n�α��� ����\n");
					retval = send(sock, log_buf, strlen(log_buf), 0);
					if (retval == SOCKET_ERROR) {
						Err_DisPlay("send_log");
						return;
					}
					printf("\n[TCP Ŭ���̾�Ʈ] %s �α���\n", clientuser->id);
					flug = true;

					LoginMenu(sock, user, clientuser, count);
					break;
				}
			}

			if (!flug) {
				strcpy(log_buf, "\nID �Ǵ� PASSWORD ����ġ\n");
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
					strcpy(log_buf, "\n���̵� �ߺ� !\n");
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
				strcpy(log_buf, "\nȸ������ �Ϸ� !\n");
				retval = send(sock, log_buf, strlen(log_buf), 0);
				if (retval == SOCKET_ERROR) {
					Err_DisPlay("send_log");
					return;
				}

				user[*count] = *clientuser;
				*count += 1;

				printf("\n[TCP Ŭ���̾�Ʈ] %s ȸ������ �Ϸ�\n", clientuser->id);
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

// �α��� ���� �޴� �Լ�
void LoginMenu(SOCKET sock, User* user, User* clientuser, int* count) {
	char log_buf[BUFSIZE];
	char temp[BUFSIZE];

	int retval = 0;
	int num = 0;

	char menu_buf[BUFSIZE];
	strcpy(menu_buf, "[ �α��� �ý��� ]\n\n1.���ӽ���\n2.ȸ��Ż��\n3.�α׾ƿ�\n\n���� : ");

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

			strcpy(log_buf, "\nŻ�� �Ϸ�\n");

			retval = send(sock, log_buf, strlen(log_buf), 0);
			if (retval == SOCKET_ERROR) {
				Err_DisPlay("send_Delete");
				return;
			}

			printf("\n[TCP Ŭ���̾�Ʈ] %s Ż��\n", clientuser->id);
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

// UpDown ���� ���� �Լ�
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

			strcat(temp, " ���� �ְ� ���� : ");
			sprintf(index, "%d\n", user[i].score);
			
			strcat(temp, index);
			strcat(temp, "\n1~100 ���� ���� �� ������ �ֽ��ϴ�.\n\n");

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
					strcpy(temp, " ������ ���߼̽��ϴ�.\n");
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

	// ���� ����Ʈ
	User user[MAX];
	ZeroMemory(&user, sizeof(user));
	
	// ������
	int count = 0;

	// ������ �ҷ����� ( ���� ����, ���� �� )
	FileLoad(user, &count);

	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		Err_Quit("WSAStartup()");
	}

	// ���� �ʱ�ȭ �Լ�
	SOCKET Listen_Sock = SockInit();

	printf("\n[TCP/IP �α��� �ý��� (UP Down Game)]\n");
	printf("\n���� ��ϵ� ���� : %d ��\n", count);

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

		// ������ ������ ������ ����
		count_temp = count;
		for (int i = 0; i < count_temp; i++) {
			user_temp[i] = user[i];
		}

		printf("\n���� ��ϵ� ���� : %d ��\n", count);

		printf("\n[TCP/IP Ŭ���̾�Ʈ ����] IP : %s , PORT : %d\n", inet_ntoa(ClientServer.sin_addr),
			ntohs(ClientServer.sin_port));

		// �α��� ���α׷� ����
		LoginSystem(Client, user, &count);

		closesocket(Client);
		printf("\n[TCP Ŭ���̾�Ʈ ���� ����] IP : %s , PORT : %d\n", inet_ntoa(ClientServer.sin_addr), ntohs(ClientServer.sin_port));

		FileSave(user, &count);
	}

	closesocket(Listen_Sock);
	WSACleanup();
	return 0;
}


// ������ �����ϱ�
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

// ������ �ҷ�����
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
