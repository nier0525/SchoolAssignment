#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <ws2tcpip.h> 

#include "listNode.h"

#define SERVERPORT 9000
#define BUFSIZE 4096
#define BUFSIZE_2 2000

enum PROTOCOL { INTRO, DOWNROAD };

struct Client_Info {
	SOCKET sock;
	SOCKADDR_IN addr;
	char buf[BUFSIZE * 2];
};

ListNode<Client_Info*>* client_list = nullptr;

Client_Info* Add_ClientInfo(SOCKET sock, SOCKADDR_IN addr) {
	Client_Info* temp = new Client_Info();
	ZeroMemory(temp, sizeof(Client_Info));
	temp->sock = sock;
	memcpy(&temp->addr, &addr, sizeof(SOCKADDR_IN));

	if (AL_Insert<Client_Info*>(client_list, temp) == ERROR) {
		return nullptr;
	}

	// 접속한 클라이언트 정보 출력
	printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(temp->addr.sin_addr), ntohs(temp->addr.sin_port));

	return temp;
}

bool Remove_ClientInfo(Client_Info* ptr) {
	// closesocket()
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));

	if (!AL_Delete<Client_Info*>(client_list, ptr)) {
		return false;
	}

	closesocket(ptr->sock);
	return true;
}

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

// 현재 디렉토리에서 파일 존재 여부를 확인한다.
BOOL SearchFile(const char *filename)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFindFile = FindFirstFile(filename, &FindFileData);
	if (hFindFile == INVALID_HANDLE_VALUE)
		return FALSE;
	else {
		FindClose(hFindFile);
		return TRUE;
	}
}

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

int Memcpy_Send(void* ptr, const void* data, size_t size, int* len) {
	memcpy(ptr, data, size);
	*len += size;
	return size;
}

int Memcpy_Recv(void* data, const void* ptr, size_t size) {
	memcpy(data, ptr, size);
	return size;
}

bool Packing_Recvn(SOCKET client_sock, char* buf) {
	int retval = 0;
	int size = 0;

	retval = recvn(client_sock, (char*)& size, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		return false;
	}
	else if (retval == 0)
		return false;

	retval = recvn(client_sock, buf, size, 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		return false;
	}
	else if (retval == 0)
		return false;

	return true;
}

int GetProtocol(char* buf) {
	int protocol;
	memcpy(&protocol, buf, sizeof(int));
	return protocol;
}

int Packing(char* buf, const int* protocol, const int* data) {
	char* ptr = buf + sizeof(int);
	int size = 0;

	ptr += Memcpy_Send(ptr, &protocol, sizeof(int), &size);
	ptr += Memcpy_Send(ptr, data, sizeof(int), &size);
	ptr = buf;

	memcpy(ptr, &size, sizeof(int));

	return size;
}

int UnPacking(const char* buf, char* data) {
	const char* ptr = buf + sizeof(int);
	int len = 0;

	ptr += Memcpy_Recv(&len, ptr, sizeof(int));
	ptr += Memcpy_Recv(data, ptr, len);

	return len;
}

int UnPacking(const char* buf, char* data , int* data2) {
	const char* ptr = buf + sizeof(int);
	int len = 0;

	ptr += Memcpy_Recv(&len, ptr, sizeof(int));
	ptr += Memcpy_Recv(data, ptr, len);
	ptr += Memcpy_Recv(data2, ptr, sizeof(int));

	return len;
}

void UnPacking(const char* buf, int* data) {
	const char* ptr = buf + sizeof(int);

	ptr += Memcpy_Recv(data, ptr, sizeof(int));
}

int main(int argc, char *argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
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

	// Create List
	AL_CreateListNode(&client_list);

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;

	while (1)
	{
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		Client_Info* ptr = Add_ClientInfo(client_sock, clientaddr);

		char filename[256];
		int totalbytes = 0;
		int currbytes = 0;

		// 클라이언트와 데이터 통신
		while (1)
		{
			int size = 0;

			PROTOCOL protocol;

			if (!Packing_Recvn(ptr->sock, ptr->buf)) {
				break;
			}

			protocol = (PROTOCOL)GetProtocol(ptr->buf);

			switch (protocol) {
			case INTRO:
				ZeroMemory(filename, 256);

				UnPacking(ptr->buf, filename);
				printf("-> 받을 파일 이름: %s\n", filename);

				if (SearchFile(filename)) {
					FILE* fp = fopen(filename, "rb");
					if (fp == NULL) {
						err_display("파일 입출력 오류");
					}
					fseek(fp, 0, SEEK_END);	// 파일 포인터 위치 이동 ( 파일포인터, 위치 이동값, 이동 시작점 )
					currbytes = ftell(fp);	// 현재 파일 포인터의 위치 ~ 파일 포인터 시작지점까지의 크기
					fclose(fp);
				}

				size = Packing(ptr->buf, (int*)INTRO, &currbytes);
				retval = send(ptr->sock, ptr->buf, size + sizeof(int), 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
				}
				continue;

			case DOWNROAD:
				UnPacking(ptr->buf, &totalbytes);
				printf("-> 받을 데이터 크기: %d\n", totalbytes);

				// 파일 열기
				FILE* fp = fopen(filename, "ab"); // append & binary mode
				if (fp == NULL) {
					err_display("파일 입출력 오류");
				}

				// 파일 데이터 받기
				int numtotal = 0;
				int down = 0;
				char buf[BUFSIZE];

				while (1)
				{
					if (!Packing_Recvn(ptr->sock, ptr->buf)) {
						break;
					}

					down = UnPacking(ptr->buf, buf);

					fwrite(buf, 1, down, fp);
					if (ferror(fp)) {
						err_display("파일 입출력 오류");
					}

					numtotal += down;
				}
				fclose(fp);
				break;
			}
			Remove_ClientInfo(ptr);
			break;
		}

		// closesocket()
	//	closesocket(client_sock);

	}

	// closesocket()
	closesocket(listen_sock);

	// Free List
	AL_FreeListNode(client_list);

	// 윈속 종료
	WSACleanup();
	return 0;
}