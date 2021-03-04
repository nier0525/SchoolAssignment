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

bool Recv(LPClientInfo ptr)
{
	int retval;
	DWORD recvbytes;
	DWORD flags = 0;
	ZeroMemory(&ptr->r_overlapped.overlapped, sizeof(ptr->r_overlapped.overlapped));
	
	ptr->r_wsabuf.buf = ptr->recv_buf + ptr->comp_recvbytes;

	if (ptr->r_sizeflag)
	{
		ptr->r_wsabuf.len = ptr->recvbytes - ptr->comp_recvbytes;
	}
	else
	{
		ptr->r_wsabuf.len = sizeof(int) - ptr->comp_recvbytes;
	}

	retval = WSARecv(ptr->sock, &ptr->r_wsabuf, 1, &recvbytes, &flags, &ptr->r_overlapped.overlapped, NULL);
	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != ERROR_IO_PENDING)
		{
			err_display("WSARecv()");
			return false;
		}
	}

	return true;
}

bool Send(LPClientInfo ptr, int size)
{
	int retval;
	DWORD sendbytes, flags;
	ZeroMemory(&ptr->s_overlapped.overlapped, sizeof(ptr->s_overlapped.overlapped));

	if (ptr->sendbytes == 0)
		ptr->sendbytes = size;

	ptr->s_wsabuf.buf = ptr->send_buf + ptr->comp_sendbytes;
	ptr->s_wsabuf.len = ptr->sendbytes - ptr->comp_sendbytes;

	retval = WSASend(ptr->sock, &ptr->s_wsabuf, 1, &sendbytes, 0, &ptr->s_overlapped.overlapped, NULL);
	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != ERROR_IO_PENDING)
		{
			err_display("WSASend()");
			return false;
		}
	}

	return true;
}

int CompleteRecv(LPClientInfo ptr, int byte)
{
	if (!ptr->r_sizeflag)
	{
		ptr->comp_recvbytes += byte;

		if (ptr->comp_recvbytes == sizeof(int));
		{
			memcpy(&ptr->recvbytes, ptr->recv_buf, sizeof(int));
			ptr->comp_recvbytes = 0;
			ptr->r_sizeflag = true;
		}

		if (!Recv(ptr))
			return SOC_ERROR;

		return SOC_FALSE;
	}

	ptr->comp_recvbytes += byte;

	if (ptr->comp_recvbytes == ptr->recvbytes)
	{
		ptr->comp_recvbytes = 0;
		ptr->recvbytes = 0;
		ptr->r_sizeflag = false;

		return SOC_TRUE;
	}
	else
	{
		if (!Recv(ptr))
			return SOC_ERROR;

		return SOC_FALSE;
	}
}

int CompleteSend(LPClientInfo ptr, int byte)
{
	ptr->comp_sendbytes += byte;
	if (ptr->comp_sendbytes == ptr->sendbytes)
	{
		ptr->comp_sendbytes = 0;
		ptr->sendbytes = 0;

		return SOC_TRUE;
	}
	
	if (!Send(ptr, ptr->sendbytes))
	{
		return SOC_ERROR;
	}

	return SOC_FALSE;
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
