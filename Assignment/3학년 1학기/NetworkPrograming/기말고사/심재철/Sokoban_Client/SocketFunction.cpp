#include "SocketFunction.h"

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

void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	char msg2[128];
	sprintf(msg2, "[오류] %s", (char*)lpMsgBuf);

	MessageBox(nullptr, msg2, msg, MB_OK);
	 
	LocalFree(lpMsgBuf);
}
// 소켓 함수 오류 출력
void err_display(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, errcode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	
	char msg[128];
	sprintf(msg,"[오류] %s", (char*)lpMsgBuf);

	MessageBox(nullptr, msg, "Error", MB_OK);

	LocalFree(lpMsgBuf);
}

// 사용자 정의 recv
int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
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

// 가변 길이 Recv
bool PacketRecv(SOCKET _sock, char* _buf)
{
	int size;

	int retval = recvn(_sock, (char*)&size, sizeof(size), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("packet size recv error()");
		return false;
	}
	else if (retval == 0)
	{
		return false;
	}

	retval = recvn(_sock, _buf, size, 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("packet data recv error()");
		return false;
	}
	else if (retval == 0)
	{
		return false;
	}

	return true;
}

// 프로토콜 수신
PROTOCOL GetProtocol(char* buf)
{
	PROTOCOL protocol;
	memcpy(&protocol, buf, sizeof(PROTOCOL));
	return protocol;
}


// Packing

void PackPacket(char* buf, PROTOCOL protocol, int& size)
{
	char* ptr = buf;
	size = 0;

	ptr = ptr + sizeof(size);

	memcpy(ptr, &protocol, sizeof(protocol));
	ptr = ptr + sizeof(protocol);
	size = size + sizeof(protocol);

	ptr = buf;
	memcpy(ptr, &size, sizeof(size));

	size = size + sizeof(size);
}

void PackPacket(char* buf, PROTOCOL protocol, int x, int y, int& size)
{
	char* ptr = buf;
	size = 0;

	ptr = ptr + sizeof(size);

	memcpy(ptr, &protocol, sizeof(protocol));
	ptr = ptr + sizeof(protocol);
	size = size + sizeof(protocol);

	memcpy(ptr, &x, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

	memcpy(ptr, &y, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

	ptr = buf;
	memcpy(ptr, &size, sizeof(size));

	size = size + sizeof(size);
}

void PackPacket(char* buf, PROTOCOL protocol, int x, int y, int x1, int y1, int& size)
{
	char* ptr = buf;
	size = 0;

	ptr = ptr + sizeof(size);

	memcpy(ptr, &protocol, sizeof(protocol));
	ptr = ptr + sizeof(protocol);
	size = size + sizeof(protocol);

	memcpy(ptr, &x, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

	memcpy(ptr, &y, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

	memcpy(ptr, &x1, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

	memcpy(ptr, &y1, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

	ptr = buf;
	memcpy(ptr, &size, sizeof(size));

	size = size + sizeof(size);
}




// UnPacking

void UnPackPacket(char* buf, char map[HEIGHT][WIDTH], char map2[HEIGHT][WIDTH], int& stage, int& stage2, int& x, int& y, int& x1, int& y1)
{
	char* ptr = buf + sizeof(PROTOCOL);
	int len;

	memcpy(&len, ptr, sizeof(int));
	ptr = ptr + sizeof(int);

	memcpy(map, ptr, len);
	ptr = ptr + len;

	memcpy(map2, ptr, len);
	ptr = ptr + len;

	memcpy(&stage, ptr, sizeof(int));
	ptr = ptr + sizeof(int);

	memcpy(&stage2, ptr, sizeof(int));
	ptr = ptr + sizeof(int);

	memcpy(&x, ptr, sizeof(int));
	ptr = ptr + sizeof(int);

	memcpy(&y, ptr, sizeof(int));
	ptr = ptr + sizeof(int);

	memcpy(&x1, ptr, sizeof(int));
	ptr = ptr + sizeof(int);

	memcpy(&y1, ptr, sizeof(int));
	ptr = ptr + sizeof(int);

}
