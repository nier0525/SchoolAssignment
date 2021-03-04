#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <stdio.h>

#define SERVER_PORT 9000
#define BUFSIZE 4096
#define MSGSIZE 512

#define INTRO_MSG "31 를 먼저 부르는 사람이 패배합니다.\n상대가 부른 수 + 3 까지 부를 수 있습니다."
#define WIN_MSG "승리하셨습니다."
#define LOSE_MSG "패배하셨습니다."

enum PROTOCOL { INTRO = 1,  ANSWER_INFO, ANSWER_RESULT };
enum RESULT { NO_WIN = -1, WIN = 1, LOSE };

void err_quit(const char *msg);																			// 에러 함수 ( 종료 )
void err_display(const char *msg);																		// 에러 함수 ( 출력 )

SOCKET socket_init();

int recvn(SOCKET s, char *buf, int len, int flags);												// 사용자 지정 Recv 함수
bool PacketRecv(SOCKET _sock, char* _buf);													// Paking 을 위한 Recv 함수
int GetProtocol(const char* _ptr);

int PackPacket(char* _buf, int _protocol, const char* _str);
int PackPacket(char* _buf, int _protocol, int result, const char* _str);

void UnPackPacket(const char* _buf, int& _data);

int main() {
	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return -1;

	SOCKET listen_sock = socket_init();

	SOCKET client_sock;
	SOCKADDR_IN client_addr;
	int addrlen;

	char buf[BUFSIZE];
	char msg[MSGSIZE];
	int size;

	PROTOCOL protocol;
	RESULT result;

	while (1) {
		// Accept
		addrlen = sizeof(SOCKADDR_IN);
		client_sock = accept(listen_sock, (SOCKADDR*)&client_addr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("Accept");
			continue;
		}

		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

		size = PackPacket(buf, (PROTOCOL)INTRO, INTRO_MSG);
		retval = send(client_sock, buf, size, 0);
		if (retval == SOCKET_ERROR) {
			err_display("Intro send");
		}

		bool flag = false;

		while (1) {
			if (!PacketRecv(client_sock, buf)) {
				break;
			}

			protocol = (PROTOCOL)GetProtocol(buf);

			switch (protocol) {
			case ANSWER_INFO:
				int answer;
				int num;
				UnPackPacket(buf, answer);

				if (answer >= 31) {
					size = PackPacket(buf, (PROTOCOL)ANSWER_RESULT, (RESULT)LOSE, LOSE_MSG);
					flag = true;
				}
				else {
					num = rand() % 3 + (answer + 1);

					if (num >= 31) {
						size = PackPacket(buf, (PROTOCOL)ANSWER_RESULT, (RESULT)WIN, WIN_MSG);
						flag = true;
					}
					else {
						sprintf(msg, "%d", num);
						size = PackPacket(buf, (PROTOCOL)ANSWER_RESULT, (RESULT)NO_WIN, msg);
					}
				}

				retval = send(client_sock, buf, size, 0);
				if (retval == SOCKET_ERROR) {
					err_display("Answer_Info Send");
					flag = true;
				}
				break;
			}
			
			if (flag) {
				break;
			}
		}

		printf("\n[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
		closesocket(client_sock);
	}

	closesocket(listen_sock);
	WSACleanup();
	return 0;
}

// 에러 함수 ( 종료 )
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

// 에러 함수 ( 출력 )
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

// 소켓 초기화 함수
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

// 사용자 정의 recv 함수
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

// Packing 을 위한 recv 함수
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

// 프로토콜 읽기 함수
int GetProtocol(const char* _ptr)
{
	int protocol;
	memcpy(&protocol, _ptr, sizeof(int));

	return protocol;
}

// Packing
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
void UnPackPacket(const char* _buf, int& _data) {
	const char* ptr = _buf + sizeof(int);

	memcpy(&_data, ptr, sizeof(int));
	ptr = ptr + sizeof(int);
}