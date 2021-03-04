#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#define REMOTEIP "255.255.255.255"
#define REMOTEPORT 9050
#define BUFSIZE 512

#define ROOMNAME_LEN 50

enum STATE {
	WAIT = 1,
	VIEW,
	SELECT,
	ROOMIP
};

enum ROOMSTATE { A = 1, B, C };

struct ROOMLIST {
	char roomname[10][ROOMNAME_LEN];
	int roomsize;
};

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

bool RoomSelect(int num, char* buf) {
	switch (num) {
	case A:
		strcpy(buf, "225.0.0.1");
		break;

	case B:
		strcpy(buf, "225.0.0.2");
		break;

	case C:
		strcpy(buf, "225.0.0.3");
		break;

	default:
		return false;
	}
	return true;
}

int main(int argc, char* argv[]) {
	int retval;

	// WSA 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != NULL) return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// 소켓 주소 구조체 초기화
	SOCKADDR_IN remoteaddr;
	ZeroMemory(&remoteaddr, sizeof(remoteaddr));
	remoteaddr.sin_family = AF_INET;
	remoteaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	remoteaddr.sin_port = htons(REMOTEPORT);
	retval = bind(sock, (SOCKADDR*)&remoteaddr, sizeof(remoteaddr));
	if (retval == SOCKET_ERROR) {
		err_quit("bind()");
	}

	// 데이터 통신에 사용할 변수
	SOCKADDR_IN peeraddr;
	int addrlen = sizeof(peeraddr);

	char buf[BUFSIZE + 1];
	int len = 0;
	bool flag = false;
	int num;

	STATE state = WAIT;

	ROOMLIST roomlist;
	ZeroMemory(&roomlist, sizeof(ROOMLIST));
	strcpy(roomlist.roomname[roomlist.roomsize++], "박용택방");
	strcpy(roomlist.roomname[roomlist.roomsize++], "오지환방");
	strcpy(roomlist.roomname[roomlist.roomsize++], "이천웅방");

	const int membersize = 3;
	int member_count[membersize];

	while (1) {
		switch (state) {
		case WAIT:
			ZeroMemory(&buf, sizeof(BUFSIZE));
			retval = recvfrom(sock, buf, BUFSIZE, 0, (SOCKADDR*)&peeraddr, &addrlen);
			if (retval == SOCKET_ERROR) {
				err_display("recvfrom()");
				continue;
			}

			num = atoi(buf);

			if (RoomSelect(num, buf)) {
				state = ROOMIP;
			}
			else {
				state = VIEW;
			}
			break;
		case VIEW:
			ZeroMemory(&buf, sizeof(BUFSIZE));
			printf("[UDP / %s : %d] 방 리스트 요청\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));
			memcpy(buf, &roomlist, sizeof(ROOMLIST));
			retval = sendto(sock, buf, sizeof(ROOMLIST), 0, (SOCKADDR*)&peeraddr, sizeof(peeraddr));
			if (retval == SOCKET_ERROR) {
				err_display("sendto()");
				continue;
			}
			state = SELECT;
			break;
		case SELECT:
			ZeroMemory(&buf, sizeof(BUFSIZE));
			retval = recvfrom(sock, buf, BUFSIZE, 0, (SOCKADDR*)&peeraddr, &addrlen);
			if (retval == SOCKET_ERROR) {
				err_display("recvfrom()");
				continue;
			}
			
			num = atoi(buf);
		
			if (RoomSelect(num, buf)) {
				state = ROOMIP;
			}
			else {
				if (num == NULL) {
					state = WAIT;
				}
				else {
					state = VIEW;
				}
			}
			break;
		case ROOMIP:
			retval = sendto(sock, buf, strlen(buf), 0, (SOCKADDR*)&peeraddr, sizeof(peeraddr));
			if (retval == SOCKET_ERROR) {
				err_display("sendto()");
				continue;
			}
			printf("[UDP / %s : %d] 방 주소 정보 송신 완료\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));
			state = WAIT;
			break;
		}
	}

	closesocket(sock);

	WSACleanup();
	return 0;
}