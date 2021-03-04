#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 4096

enum PROTOCOL { INTRO, DOWNROAD };

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

int Memcpy_Send(void* ptr, const void* data, size_t size, int* len) {
	memcpy(ptr, data, size);
	*len += size;
	return size;
}

int Memcpy_Recv(void* data, const void* ptr, size_t size) {
	memcpy(data, ptr, size);
	return size;
}

int GetProtocol(char* buf) {
	int protocol;
	memcpy(&protocol, buf, sizeof(int));
	return protocol;
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

int Packing(char* buf, const int* protocol, const char* data) {
	char* ptr = buf + sizeof(int);
	int size = 0;
	int len = strlen(data);

	ptr += Memcpy_Send(ptr, &protocol, sizeof(int), &size);
	ptr += Memcpy_Send(ptr, &len, sizeof(int), &size);
	ptr += Memcpy_Send(ptr, data, len, &size);

	ptr = buf;

	memcpy(ptr, &size, sizeof(int));

	return size;
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

int Packing(char* buf, const int* protocol, const char* data, const int* num) {
	char* ptr = buf + sizeof(int);
	int size = 0;
	int len = *num;
	
	ptr += Memcpy_Send(ptr, &protocol, sizeof(int), &size);
	ptr += Memcpy_Send(ptr, &len, sizeof(int), &size);
	ptr += Memcpy_Send(ptr, data, len, &size);
	ptr += Memcpy_Send(ptr, num, sizeof(int), &size);

	ptr = buf;

	memcpy(ptr, &size, sizeof(int));

	return size;
}

void UnPacking(const char* buf, int* data) {
	const char* ptr = buf + sizeof(int);

	ptr += Memcpy_Recv(data, ptr, sizeof(int));
}


int main(int argc, char *argv[])
{
	int retval;

	if (argc < 2)
	{
		printf("Usage: %s [FileName]\n", argv[0]);
		return -1;
	}
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
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

	// 데이터 통신에 사용할 변수
	char buf[BUFSIZE * 2];

	FILE *fp = fopen(argv[1], "rb");
	if (fp == NULL) {
		err_display("파일 입출력 오류");
		return -1;
	}

	while (1) {
		int size = 0;
		char filename[256];

		PROTOCOL protocol;

		ZeroMemory(filename, 256);
		sprintf(filename, argv[1]);

		printf("%s\n", argv[1]);

		size = Packing(buf, (int*)INTRO, filename);

		retval = send(sock, buf, size + sizeof(int), 0);
		if (retval == SOCKET_ERROR) err_display("send()");

		if (!Packing_Recvn(sock, buf)) {
			break;
		}

		int currbytes;
		UnPacking(buf, &currbytes);

		// 파일 크기 얻기
		fseek(fp, 0, SEEK_END);
		int totalbytes = ftell(fp);
		totalbytes -= currbytes;

		size = Packing(buf, (int*)DOWNROAD, &totalbytes);
		retval = send(sock, buf, size + sizeof(int), 0);
		if (retval == SOCKET_ERROR) err_display("send()");

		// 파일 데이터 전송에 사용할 변수
		int numread;
		int numtotal = 0;
		char f_buf[BUFSIZE];

		// 파일 데이터 보내기
		fseek(fp, currbytes, SEEK_SET); // 파일 포인터를 전송 시작 위치로 이동
		while (1)
		{
			numread = fread(f_buf, 1, BUFSIZE, fp);

			if (numread > 0) {
				size = Packing(buf, (int*)DOWNROAD, f_buf, &numread);

				retval = send(sock, buf, size + sizeof(int), 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
				}

				numtotal += numread;
				printf("보낸 데이터 : %d 바이트\n", numtotal); // 전송 상황을 표시
				Sleep(500); // 전송 중단 실험을 위해 속도를 느리게 함
			}
			else if (numread == 0 && numtotal == totalbytes) {
				printf("\n파일 전송 완료!: %d 바이트\n", numtotal);
				break;
			}
			else {
				err_display("파일 입출력 오류");
				break;
			}
		}
		break;
	}

	fclose(fp);
	closesocket(sock);
	// 윈속 종료
	WSACleanup();
	return 0;
}