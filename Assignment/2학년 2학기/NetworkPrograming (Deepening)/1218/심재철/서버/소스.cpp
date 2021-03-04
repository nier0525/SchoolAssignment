#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <stdio.h>
#include <time.h>

#define SERVER_PORT 6470
#define BUFSIZE 4096
#define MSGSIZE 512

#define INTRO_MSG "���ھ߱����� �Դϴ�. ���� �����ư��� ������ ����, ��뺸�� ������ ����� ������ ���ߴ� ����� �¸��մϴ�."
#define WAIT_MSG "����� ������ ��ٸ��� �ֽ��ϴ� . . ."
#define WAIT_QUESTION_MSG "��밡 ������ �Է��ϰ� �ֽ��ϴ� . . . "
#define QUESTION_MSG "0~9 ���� �ߺ����� �ʴ� �� 3 ���� �Է��ϼ���."
#define GAMESTART_MSG "��밡 ������ �½��ϴ�. 0~9 ���� ���� �� �ߺ����� �ʴ� 3 ���� ���� �Է��ϼ���."
#define WAIT_PLAYER_ANSWER_MSG "��밡 ������ Ǯ�� �ֽ��ϴ�. ��ø� ��ٷ��ּ��� . . ."
#define WIN_MSG "����� �¸��ϼ̽��ϴ�."
#define LOSE_MSG "����� �й��ϼ̽��ϴ�."
#define NO_WIN_MSG "�����ϴ�."
#define GIVEUP_MSG "��밡 ������ �������ϴ�."
#define NUM_ERROR_MSG "�߸��� �Է��Դϴ�."

enum STATE {
	S_INIT = 1,
	S_INTRO,
	S_WAITQUESTION,
	S_QUESTION,
	S_QUESTIONSELECT,
	S_MYTRUN,
	S_OTHERTRUN,
	S_GAMERESULT,
	S_PARTRESET,
	S_DISCONNECTION
};

enum PROTOCOL {
	WAIT = -1,
	INTRO = 1,
	WAIT_QUESTION,
	QUESTION,
	QUESTION_SELECT,
	QUESTION_ERROR,
	GAME_START,
	MY_TRUN,
	OTHER_TRUN,
	VALUE_SELECT,
	VALUE_ERROR,
	VALUE_RESULT,
	PART_GIVEUP,
	GAME_RESULT
};

enum RESULT {
	GIVEUP = -1,
	NO_WIN = 1,
	WIN,
	LOSE
};

struct _ClientInfo {
	SOCKET sock;
	SOCKADDR_IN addr;
	char buf[BUFSIZE];

	STATE state;

	int answer[3];
	int count;
	bool trun;

	HANDLE hWait;
	HANDLE hTrun;
	HANDLE hReset;

	_ClientInfo* part;
};

_ClientInfo* client_info[100];
int client_count = 0;

CRITICAL_SECTION cs;

_ClientInfo* AddClientInfo(SOCKET sock, SOCKADDR_IN addr);
void RemoveClientInfo(_ClientInfo* ptr);

bool MatchPart(_ClientInfo* ptr);

// ���� ���� ���� �Լ�

void err_quit(const char *msg);																			// ���� �Լ� ( ���� )
void err_display(const char *msg);																		// ���� �Լ� ( ��� )

SOCKET socket_init();

int recvn(SOCKET s, char *buf, int len, int flags);
bool PacketRecv(SOCKET _sock, char* _buf);
int GetProtocol(const char* _ptr);

int PackPacket(char* _buf, int _protocol);
int PackPacket(char* _buf, int _protocol, const char* _str);
int PackPacket(char* _buf, int _protocol, int _result, const char* _str);

int UnPackPacket(const char* _buf, char* _data);

DWORD WINAPI ClientProcess(LPVOID arg);

// main

int main() {

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return -1;

	SOCKET listen_sock = socket_init();
	InitializeCriticalSection(&cs);

	SOCKET sock;
	SOCKADDR_IN addr;
	int addrlen;

	srand(time(nullptr));

	while (1) {
		addrlen = sizeof(SOCKADDR_IN);
		sock = accept(listen_sock, (SOCKADDR*)&addr, &addrlen);
		if (sock == INVALID_SOCKET) {
			err_display("Accept");
			continue;
		}

		_ClientInfo* ptr = AddClientInfo(sock, addr);
		MatchPart(ptr);

		HANDLE hThraed = CreateThread(nullptr, 0, ClientProcess, ptr, 0, nullptr);
		if (hThraed == nullptr) {
			RemoveClientInfo(ptr);
		}
		else {
			CloseHandle(hThraed);
		}
	}

	DeleteCriticalSection(&cs);
	closesocket(listen_sock);
	WSACleanup();
	return 0;
}

DWORD WINAPI ClientProcess(LPVOID arg) {
	_ClientInfo* ptr = (_ClientInfo*)arg;

	int retval;
	int size;
	int strike, ball, out;
	char msg[MSGSIZE];

	bool endflag = false;

	while (1) {
		switch (ptr->state) {
		case S_INIT:
			size = PackPacket(ptr->buf, (PROTOCOL)WAIT, WAIT_MSG);
			retval = send(ptr->sock, ptr->buf, size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("Wait Send");
				ptr->state = S_DISCONNECTION;
			}

			WaitForSingleObject(ptr->hWait, INFINITE);
			ptr->state = S_INTRO;
			break;

		case S_INTRO:
			size = PackPacket(ptr->buf, (PROTOCOL)INTRO, INTRO_MSG);
			retval = send(ptr->sock, ptr->buf, size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("Intro Send");
				ptr->state = S_DISCONNECTION;
			}

			if (ptr->part != nullptr && ptr->state != S_DISCONNECTION) {
				SetEvent(ptr->part->hWait);
				if (ptr->trun)
					ptr->state = S_QUESTION;
				else
					ptr->state = S_WAITQUESTION;
			}
			else {
				ptr->state = S_DISCONNECTION;
			}
			break;

		case S_WAITQUESTION:
			size = PackPacket(ptr->buf, (PROTOCOL)WAIT_QUESTION, WAIT_QUESTION_MSG);
			retval = send(ptr->sock, ptr->buf, size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("Wait Question Send");
				ptr->state = S_DISCONNECTION;
			}

			WaitForSingleObject(ptr->hTrun, INFINITE);
			if (ptr->part == nullptr || ptr->part->state == S_DISCONNECTION) {
				ptr->state = S_PARTRESET;
			}
			else {
				size = PackPacket(ptr->buf, (PROTOCOL)GAME_START, GAMESTART_MSG);
				retval = send(ptr->sock, ptr->buf, size, 0);
				if (retval == SOCKET_ERROR) {
					err_display("Game Start Send");
					ptr->state = S_DISCONNECTION;
				}
				else {
					ptr->state = S_OTHERTRUN;
				}
			}
			break;

		case S_QUESTION:
			size = PackPacket(ptr->buf, (PROTOCOL)QUESTION, QUESTION_MSG);
			retval = send(ptr->sock, ptr->buf, size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("Question Select Send");
				ptr->state = S_DISCONNECTION;
			}

			ptr->state = S_QUESTIONSELECT;
			break;

		case S_QUESTIONSELECT:
			size = PackPacket(ptr->buf, (PROTOCOL)QUESTION_SELECT);
			retval = send(ptr->sock, ptr->buf, size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("Question Select Send");
				ptr->state = S_DISCONNECTION;
			}

			if (!PacketRecv(ptr->sock, ptr->buf)) {
				ptr->state = S_DISCONNECTION;
			}

			switch ((PROTOCOL)GetProtocol(ptr->buf)) {
			case QUESTION_SELECT:
				int len;
				bool error;
				error = false;
				ZeroMemory(msg, sizeof(msg));
				ZeroMemory(ptr->answer, sizeof(ptr->answer));
				len = UnPackPacket(ptr->buf, msg);

				if (len == 3) {
					for (int i = 0; i < 3; i++) {
						ptr->answer[i] = (int)msg[i] - 48;
						for (int j = 0; j < i; j++) {
							if (ptr->answer[i] == ptr->answer[j]) {
								error = true;
								break;
							}
						}
						if (error) {
							break;
						}
					}
				}
				else {
					error = true;
				}

				if (error) {
					size = PackPacket(ptr->buf, (PROTOCOL)QUESTION_ERROR, NUM_ERROR_MSG);
					retval = send(ptr->sock, ptr->buf, size, 0);
					if (retval == SOCKET_ERROR) {
						err_display("Question Error Send");
						ptr->state = S_DISCONNECTION;
					}
				}
				else {
					size = PackPacket(ptr->buf, (PROTOCOL)MY_TRUN, WAIT_PLAYER_ANSWER_MSG);
					retval = send(ptr->sock, ptr->buf, size, 0);
					if (retval == SOCKET_ERROR) {
						err_display("My Turn Send");
						ptr->state = S_DISCONNECTION;
					}

					SetEvent(ptr->part->hTrun);
					WaitForSingleObject(ptr->hTrun, INFINITE);
					if (ptr->part == nullptr || ptr->part->state == S_DISCONNECTION) {
						WaitForSingleObject(ptr->hWait, INFINITE);
						ptr->state = S_PARTRESET;
					}
					else {
						if (ptr->count == 0)
							ptr->state = S_WAITQUESTION;
						else
							ptr->state = S_GAMERESULT;
					}
				}
				break;
			}
			break;

		case S_OTHERTRUN:
			size = PackPacket(ptr->buf, (PROTOCOL)OTHER_TRUN);
			retval = send(ptr->sock, ptr->buf, size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("Other Trun Send");
				ptr->state = S_DISCONNECTION;
			}

			if (!PacketRecv(ptr->sock, ptr->buf)) {
				ptr->state = S_DISCONNECTION;
			}

			switch ((PROTOCOL)GetProtocol(ptr->buf)) {
			case VALUE_SELECT:
				strike = ball = out = 0;
				int len;
				bool nohit;
				bool error;
				error = false;
				ZeroMemory(msg, sizeof(msg));
				len = UnPackPacket(ptr->buf, msg);

				if (len == 3) {
					int num[3];
					for (int i = 0; i < 3; i++) {
						num[i] = (int)msg[i] - 48;
						for (int j = 0; j < i; j++) {
							if (num[i] == num[j]) {
								error = true;
								break;
							}
						}
						if (error) {
							break;
						}
					}

					if (!error) {
						ptr->count++;

						for (int i = 0; i < 3; i++) {
							nohit = false;
							for (int j = 0; j < 3; j++) {
								if (ptr->part->answer[i] == num[j]) {
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
				}
				else {
					error = true;
				}

				if (error) {
					size = PackPacket(ptr->buf, (PROTOCOL)VALUE_ERROR, NUM_ERROR_MSG);
					retval = send(ptr->sock, ptr->buf, size, 0);
					if (retval == SOCKET_ERROR) {
						err_display("Value Error Send");
						ptr->state = S_DISCONNECTION;
					}
				}
				else {
					if (strike == 3) {
						sprintf(msg, "%d ȸ �õ����� ������ ���߼̽��ϴ�.", ptr->count);

						size = PackPacket(ptr->buf, (PROTOCOL)VALUE_RESULT, msg);
						retval = send(ptr->sock, ptr->buf, size, 0);
						if (retval == SOCKET_ERROR) {
							err_display("Value Result Send");
							ptr->state = S_DISCONNECTION;
						}

						SetEvent(ptr->part->hTrun);

						if (ptr->part != nullptr) {
							if (!ptr->trun) {
								ptr->trun = true;
								ptr->state = S_QUESTION;
							}
							else {
								ptr->state = S_GAMERESULT;
							}
						}
						else {
							ptr->state = S_PARTRESET;
						}
					}
					else {
						sprintf(msg, "Strike : %d, Ball : %d, Out : %d", strike, ball, out);

						size = PackPacket(ptr->buf, (PROTOCOL)VALUE_RESULT, msg);
						retval = send(ptr->sock, ptr->buf, size, 0);
						if (retval == SOCKET_ERROR) {
							err_display("Value Result Send");
							ptr->state = S_DISCONNECTION;
						}
					}
				}
				break;
			}
			break;

		case S_GAMERESULT:
			if (ptr->count < ptr->part->count) {
				size = PackPacket(ptr->buf, (PROTOCOL)GAME_RESULT, (RESULT)WIN, WIN_MSG);
			}
			else if (ptr->count > ptr->part->count) {
				size = PackPacket(ptr->buf, (PROTOCOL)GAME_RESULT, (RESULT)LOSE, LOSE_MSG);
			}
			else {
				size = PackPacket(ptr->buf, (PROTOCOL)GAME_RESULT, (RESULT)NO_WIN, NO_WIN_MSG);
			}

			retval = send(ptr->sock, ptr->buf, size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("Game Result Send");
			}
			endflag = true;
			break;

		case S_PARTRESET:
			WaitForSingleObject(ptr->hReset, INFINITE);

			size = PackPacket(ptr->buf, (PROTOCOL)GIVEUP, GIVEUP_MSG);
			retval = send(ptr->sock, ptr->buf, size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("Give Up Send");
				endflag = true;
			}

			ptr->part = nullptr;
			ptr->count = 0;
			MatchPart(ptr);
			ptr->state = S_INIT;
			break;

		case S_DISCONNECTION:
			if (ptr->part != nullptr) {
				if (ptr->part->state != S_INIT) {
					SetEvent(ptr->part->hTrun);
				}
				ptr->part->trun = true;
			}
			endflag = true;
			break;
		}

		if (endflag) {
			if (!PacketRecv(ptr->sock, ptr->buf)) {
				if (ptr->part != nullptr)
					SetEvent(ptr->part->hReset);

				RemoveClientInfo(ptr);
				break;
			}
		}
	}

	return 0;
}

bool MatchPart(_ClientInfo* ptr) {
	EnterCriticalSection(&cs);
	for (int i = 0; i < client_count; i++) {
		if (ptr != client_info[i] && client_info[i]->part == nullptr) {
			printf("\nMatch\n");
			printf("%d\n", ntohs(client_info[i]->addr.sin_port));
			client_info[i]->part = ptr;
			ptr->part = client_info[i];
			ptr->trun = false;

			SetEvent(ptr->part->hWait);
			LeaveCriticalSection(&cs);
			return true;
		}
	}
	ptr->trun = true;
	LeaveCriticalSection(&cs);
	return false;
}

_ClientInfo* AddClientInfo(SOCKET sock, SOCKADDR_IN addr) {
	EnterCriticalSection(&cs);
	_ClientInfo* ptr = new _ClientInfo();
	ZeroMemory(ptr, sizeof(_ClientInfo));
	ptr->sock = sock;
	memcpy(&ptr->addr, &addr, sizeof(SOCKADDR_IN));
	ptr->state = S_INIT;

	ZeroMemory(ptr->answer, sizeof(ptr->answer));
	ptr->count = 0;
	ptr->trun = false;

	ptr->part = nullptr;
	ptr->hWait = CreateEvent(nullptr, false, false, nullptr);
	ptr->hTrun = CreateEvent(nullptr, false, false, nullptr);
	ptr->hReset = CreateEvent(nullptr, false, false, nullptr);

	client_info[client_count++] = ptr;
	LeaveCriticalSection(&cs);

	printf("\n[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));
	return ptr;
}

void RemoveClientInfo(_ClientInfo* ptr) {
	printf("\n[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));

	EnterCriticalSection(&cs);
	for (int i = 0; i < client_count; i++) {
		if (client_info[i] == ptr) {
			if (client_info[i]->part != nullptr)
				client_info[i]->part->part = nullptr;

			closesocket(client_info[i]->sock);
			CloseHandle(client_info[i]->hWait);
			CloseHandle(client_info[i]->hTrun);
			CloseHandle(client_info[i]->hReset);

			delete client_info[i];

			for (int j = i; j < client_count - 1; j++) {
				client_info[j] = client_info[j + 1];
			}
			break;
		}
	}

	client_count--;
	LeaveCriticalSection(&cs);
}

// ���� ��� ���� �Լ�

// ���� �Լ� ( ���� )
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

// ���� �Լ� ( ��� )
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

// ���� �ʱ�ȭ �Լ�
SOCKET socket_init() {
	int retval;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVER_PORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr,
		sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	return listen_sock;
}

// ����� ���� recv �Լ�
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

// Packing �� ���� recv �Լ�
bool PacketRecv(SOCKET _sock, char* _buf)
{
	int size;

	int retval = recvn(_sock, (char*)&size, sizeof(size), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv error()");
		return false;
	}
	else if (retval == 0)
	{
		return false;
	}

	retval = recvn(_sock, _buf, size, 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv error()");
		return false;

	}
	else if (retval == 0)
	{
		return false;
	}

	return true;
}

// �������� �б� �Լ�
int GetProtocol(const char* _ptr)
{
	int protocol;
	memcpy(&protocol, _ptr, sizeof(int));

	return protocol;
}

// Packing
int PackPacket(char* _buf, int _protocol) {
	int size = 0;
	char* ptr = _buf;

	ptr = ptr + sizeof(int);

	memcpy(ptr, &_protocol, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

	ptr = _buf;
	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}

int PackPacket(char* _buf, int _protocol, const char* _str) {
	int size = 0;
	char* ptr = _buf;
	int len = strlen(_str);

	ptr = ptr + sizeof(int);

	memcpy(ptr, &_protocol, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

	memcpy(ptr, &len, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

	memcpy(ptr, _str, len);
	ptr = ptr + len;
	size = size + len;

	ptr = _buf;
	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}

int PackPacket(char* _buf, int _protocol, int _result, const char* _str) {
	int size = 0;
	char* ptr = _buf;
	int len = strlen(_str);

	ptr = ptr + sizeof(int);

	memcpy(ptr, &_protocol, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

	memcpy(ptr, &_result, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

	memcpy(ptr, &len, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

	memcpy(ptr, _str, len);
	ptr = ptr + len;
	size = size + len;

	ptr = _buf;
	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}

// UnPacking
int UnPackPacket(const char* _buf, char* _data) {
	const char* ptr = _buf + sizeof(int);
	int len = 0;

	memcpy(&len, ptr, sizeof(int));
	ptr = ptr + sizeof(int);

	memcpy(_data, ptr, len);
	ptr = ptr + len;

	return len;
}
