#include "MyFunction.h"

// 소켓 함수 오류 출력 후 종료
void err_quit(const char* msg)
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
void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// 소켓 함수 오류 출력
void err_display(const int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[Error] %s", (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

PROTOCOL GetProtocol(const char* buf)
{
	PROTOCOL protocol;
	memcpy(&protocol, buf, sizeof(PROTOCOL));
	return protocol;
}

int memcpy_send(void* ptr, const void* dst, size_t size, int* len) {
	memcpy(ptr, dst, size);	// 메모리 복사
	*len += size;						// 메모리의 크기를 인자값으로 받은 크기만큼 증가
	return size;						// 인자값으로 받은 크기 리턴
}

int memcpy_recv(void* dst, const void* ptr, size_t size) {
	memcpy(dst, ptr, size);	// 메모리 복사
	return size;						// 인자값으로 받은 크기 리턴
}

int Message_Recv(ClientInfo* ptr)
{
	int retval = recv(ptr->sock, ptr->recv_buf + ptr->comp_recv_bytes, ptr->recv_bytes - ptr->comp_recv_bytes, 0);
	
	if (retval == SOCKET_ERROR) return ERROR_DISCONNECTED;
	else if (retval == 0) return DISCONNECTED;
	else
	{
		ptr->comp_recv_bytes += retval;
		if (ptr->comp_recv_bytes == ptr->recv_bytes)
		{
			ptr->comp_recv_bytes = ptr->recv_bytes = 0;
			return SOCKET_TRUE;
		}
		else
		{
			return SOCKET_FALSE;
		}
	}
}

int Packing_Recv(ClientInfo* ptr)
{
	if (!ptr->sizeflag)
	{
		ptr->recv_bytes = sizeof(int);
		int retval = Message_Recv(ptr);

		switch (retval)
		{
		case SOCKET_TRUE:
			memcpy(&ptr->recv_bytes, ptr->recv_buf, sizeof(int));
			ptr->sizeflag = true;
			return SOCKET_FALSE;

		case SOCKET_FALSE:
			return SOCKET_FALSE;

		case ERROR_DISCONNECTED:
			err_display("recv error()");
			return DISCONNECTED;

		case DISCONNECTED:
			return DISCONNECTED;
		}
	}

	else
	{
		int retval = Message_Recv(ptr);

		switch (retval)
		{
		case SOCKET_TRUE:
			ptr->sizeflag = false;
			return SOCKET_TRUE;

		case SOCKET_FALSE:
			return SOCKET_FALSE;

		case ERROR_DISCONNECTED:
			err_display("recv error()");
			return DISCONNECTED;

		case DISCONNECTED:
			return DISCONNECTED;
		}
	}
}




int Packing(char* buf, const PROTOCOL protocol, const char* str1) {
	int size = 0;
	char* ptr = buf + sizeof(int);
	int len = strlen(str1);

	ptr += memcpy_send(ptr, &protocol, sizeof(PROTOCOL), &size);

	ptr += memcpy_send(ptr, &len, sizeof(int), &size);
	ptr += memcpy_send(ptr, str1, len, &size);

	ptr = buf;
	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}

int Packing(char* buf, const PROTOCOL protocol, const RESULT result, const char* str1) {
	int size = 0;
	char* ptr = buf + sizeof(int);
	int len = strlen(str1);

	ptr += memcpy_send(ptr, &protocol, sizeof(PROTOCOL), &size);
	ptr += memcpy_send(ptr, &result, sizeof(RESULT), &size);

	ptr += memcpy_send(ptr, &len, sizeof(int), &size);
	ptr += memcpy_send(ptr, str1, len, &size);

	ptr = buf;
	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}

int Packing(char* buf, const PROTOCOL protocol, const char* str1, const char* str2) {
	int size = 0;
	char* ptr = buf + sizeof(int);
	int len = strlen(str1);
	int len2 = strlen(str2);

	ptr += memcpy_send(ptr, &protocol, sizeof(PROTOCOL), &size);

	ptr += memcpy_send(ptr, &len, sizeof(int), &size);
	ptr += memcpy_send(ptr, str1, len, &size);
	ptr += memcpy_send(ptr, &len2, sizeof(int), &size);
	ptr += memcpy_send(ptr, str2, len2, &size);

	ptr = buf;
	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}



void UnPacking(const char* buf, char* str) {
	const char* ptr = buf + sizeof(PROTOCOL);
	int len = 0;

	ptr += memcpy_recv(&len, ptr, sizeof(int));
	ptr += memcpy_recv(str, ptr, len);
}

void UnPacking(const char* buf, char* str, char* str2) {
	const char* ptr = buf + sizeof(PROTOCOL);
	int len = 0;
	int len2 = 0;

	ptr += memcpy_recv(&len, ptr, sizeof(int));
	ptr += memcpy_recv(str, ptr, len);
	ptr += memcpy_recv(&len2, ptr, sizeof(int));
	ptr += memcpy_recv(str2, ptr, len2);
}
