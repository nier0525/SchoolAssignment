#include "PackingFunction.h"

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
	printf("[%s] %s", msg, (LPCTSTR)lpMsgBuf);
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
	printf("[오류] %s", (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}


// WSA Recv
bool Recv(ClientInfo* _ptr)
{
	int retval;
	DWORD recvbytes;
	DWORD flags = 0;

	ZeroMemory(&_ptr->r_overlapped.overlapped, sizeof(_ptr->r_overlapped.overlapped));

	_ptr->r_wsabuf.buf = _ptr->recvbuf + _ptr->comp_recvbytes;

	if (_ptr->r_sizeflag)
	{
		_ptr->r_wsabuf.len = _ptr->recvbytes - _ptr->comp_recvbytes;
	}
	else
	{
		_ptr->r_wsabuf.len = sizeof(int) - _ptr->comp_recvbytes;
	}

	retval = WSARecv(_ptr->sock, &_ptr->r_wsabuf, 1, &recvbytes,
		&flags, &_ptr->r_overlapped.overlapped, nullptr);
	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{			
			err_display("WSARecv()");
			//RemoveClientInfo(_ptr);
			return false;
		}
	}

	return true;
}

// 가변길이 Recv 받기
int CompleteRecv(ClientInfo* _ptr, int _completebyte)
{

	if (!_ptr->r_sizeflag)
	{
		_ptr->comp_recvbytes += _completebyte;

		if (_ptr->comp_recvbytes == sizeof(int))
		{
			memcpy(&_ptr->recvbytes, _ptr->recvbuf, sizeof(int));
			_ptr->comp_recvbytes = 0;
			_ptr->r_sizeflag = true;
		}

		if (!Recv(_ptr))
		{
			return SOC_ERROR;
		}

		return SOC_FALSE;
	}

	_ptr->comp_recvbytes += _completebyte;

	if (_ptr->comp_recvbytes == _ptr->recvbytes)
	{
		_ptr->comp_recvbytes = 0;
		_ptr->recvbytes = 0;
		_ptr->r_sizeflag = false;

		return SOC_TRUE;
	}
	else
	{
		if (!Recv(_ptr))
		{
			return SOC_ERROR;
		}

		return SOC_FALSE;
	}
}

// WSA Send
bool Send(ClientInfo* _ptr, int _size)
{
	int retval;
	DWORD sendbytes;
	DWORD flags;

	ZeroMemory(&_ptr->s_overlapped.overlapped, sizeof(_ptr->s_overlapped.overlapped));
	if (_ptr->sendbytes == 0)
	{
		_ptr->sendbytes = _size;
	}

	_ptr->s_wsabuf.buf = _ptr->sendbuf + _ptr->comp_sendbytes;
	_ptr->s_wsabuf.len = _ptr->sendbytes - _ptr->comp_sendbytes;

	retval = WSASend(_ptr->sock, &_ptr->s_wsabuf, 1, &sendbytes,
		0, &_ptr->s_overlapped.overlapped, nullptr);
	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			err_display("WSASend()");
			//RemoveClientInfo(_ptr);
			return false;
		}
	}

	return true;
}

// 가변길이 Send 보내기
int CompleteSend(ClientInfo* _ptr, int _completebyte)
{
	_ptr->comp_sendbytes += _completebyte;
	if (_ptr->comp_sendbytes == _ptr->sendbytes)
	{
		_ptr->comp_sendbytes = 0;
		_ptr->sendbytes = 0;

		return SOC_TRUE;
	}
	if (!Send(_ptr, _ptr->sendbytes))
	{
		return SOC_ERROR;
	}

	return SOC_FALSE;
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
 
void PackPacket(char* buf, PROTOCOL protocol, char map[HEIGHT][WIDTH], char map2[HEIGHT][WIDTH], int stage, int stage2, int x, int y, int x1, int y1, int& size)
{
	char* ptr = buf;
	int len;

	size = 0;
	len = HEIGHT * WIDTH;

	ptr = ptr + sizeof(size);

	memcpy(ptr, &protocol, sizeof(protocol));
	ptr = ptr + sizeof(protocol);
	size = size + sizeof(protocol);

	memcpy(ptr, &len, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

	memcpy(ptr, map, len);
	ptr = ptr + len;
	size = size + len;

	memcpy(ptr, map2, len);
	ptr = ptr + len;
	size = size + len;

	memcpy(ptr, &stage, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

	memcpy(ptr, &stage2, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

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
void UnPackPacket(char* buf, int& x, int& y)
{
	char* ptr = buf + sizeof(PROTOCOL);
	int len;

	memcpy(&x, ptr, sizeof(int));
	ptr = ptr + sizeof(int);

	memcpy(&y, ptr, sizeof(int));
	ptr = ptr + sizeof(int);
}

void UnPackPacket(char* buf, int& x, int& y, int& x1, int& y1)
{
	char* ptr = buf + sizeof(PROTOCOL);
	int len;

	memcpy(&x, ptr, sizeof(int));
	ptr = ptr + sizeof(int);

	memcpy(&y, ptr, sizeof(int));
	ptr = ptr + sizeof(int);

	memcpy(&x1, ptr, sizeof(int));
	ptr = ptr + sizeof(int);

	memcpy(&y1, ptr, sizeof(int));
	ptr = ptr + sizeof(int);
}