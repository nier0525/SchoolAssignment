#include "MyFunction.h"

// 소켓 초기화 함수
void ListenSocket_Init(SOCKET& sock, SOCKADDR_IN& addr)
{
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	memcpy(&addr, &serveraddr, sizeof(SOCKADDR_IN));

	// bind()
	int retval = bind(sock, (SOCKADDR*)&addr, sizeof(addr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// Non Blocking
	u_long on = 1;
	retval = ioctlsocket(sock, FIONBIO, &on);
	if (retval == SOCKET_ERROR) err_display("ioctlsocket()");
}

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

// 프로토콜 수신
PROTOCOL GetProtocol(char* buf) 
{
	PROTOCOL protocol;
	memcpy(&protocol, buf, sizeof(PROTOCOL));

	return protocol;
}

// 가변 길이 패킹 함수
int memcpy_send(void* ptr, const void* dst, size_t size, int* len)
{
	memcpy(ptr, dst, size);	// 메모리 복사
	*len += size;						// 메모리의 크기를 인자값으로 받은 크기만큼 증가
	return size;						// 인자값으로 받은 크기 리턴
}

// 가변 길이 언패킹 함수
int memcpy_recv(void* dst, const void* ptr, size_t size) 
{
	memcpy(dst, ptr, size);	// 메모리 복사
	return size;						// 인자값으로 받은 크기 리턴
}

// 논 블로킹 수신 함수
int Message_Recv(Client_Info* ptr)
{
	// 한 번에 다 받는 다는 보장이 없기 때문에 나누어서 받는 작업
	// 이미 받은 데이터의 크기를 제외하고 받는다.
	int retval = recv(ptr->sock, ptr->recv_buf + ptr->total_recvbytes, ptr->recvbytes - ptr->total_recvbytes, 0);
	if (retval == SOCKET_ERROR)
	{
		return ERROR_DISCONNECTED;
	}
	else if (retval == 0)
	{
		return DISCONNECTED;
	}
	else {
		// 현재 받은 받은 크기만큼 더해준다.
		ptr->total_recvbytes += retval;
		// 수신 버퍼에서 모든 데이터를 불러왔을 경우
		if (ptr->total_recvbytes == ptr->recvbytes)
		{
			ptr->total_recvbytes = ptr->recvbytes = 0;
			return SOCKET_TRUE;
		}
		// 아직 수신 버퍼에 데이터가 남아 있는 경우
		else
		{
			return SOCKET_FALSE;
		}
	}
}

// 논 블로킹 송신 함수
int Message_Send(Client_Info* ptr)
{
	// 송신 역시 예외의 경우가 분명히 있고, 한 번에 보낸다는 보장은 없다.
	// 이미 보낸 송신 데이터의 크기를 제외하고 다시 보낸다.
	int retval = send(ptr->sock, ptr->send_buf + ptr->total_sendbytes, ptr->sendbytes - ptr->total_sendbytes, 0);
	if (retval == SOCKET_ERROR)
	{
		return ERROR_DISCONNECTED;
	}
	else if (retval == 0)
	{
		return DISCONNECTED;
	}
	else
	{
		// 보낸 송신 데이터만큼 더한다.
		ptr->total_sendbytes += retval;

		// 송신 할 데이터를 모두 보낸 경우
		if (ptr->sendbytes == ptr->total_sendbytes)
		{
			ptr->total_sendbytes = ptr->sendbytes = 0;
			return SOCKET_TRUE;
		}
		// 아직 송신 할 데이터가 남은 경우
		else
		{
			return SOCKET_FALSE;
		}
	}
}

// 가변 길이 수신
int Packing_Recv(Client_Info* ptr)
{
	// 가변 길이 패킷의 전체 크기를 먼저 받는다.
	if (!ptr->recv_flag)
	{
		// 패킷의 가장 앞에 위치한 전체 크기는 int 형이기 때문에 딱 int 형 만큼만 받는다.
		ptr->recvbytes = sizeof(int);
		int retval = Message_Recv(ptr);

		switch (retval)
		{
		// 패킷의 전체 크기 = 수신할 데이터 크기 임으로 바로 복사해준다.
		// 전체 크기를 알아냈으니 recv_flag 를 true 로 바꿔준다.
		case SOCKET_TRUE:
			memcpy(&ptr->recvbytes, ptr->recv_buf, sizeof(int));
			ptr->recv_flag = true;
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

	// 가변 길이의 전체 크기를 받은 후
	else
	{
		// 알아 낸 가변 길이의 전체 크기만큼 수신한다.
		int retval = Message_Recv(ptr);

		switch (retval)
		{
		// 수신 데이터를 모두 받아냈으면 다시 recv_flag 를 false 로 초기화 해준다.
		case SOCKET_TRUE:
			ptr->recv_flag = false;
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

// 송신 상태 체크
int Result_Send(Client_Info* ptr)
{
	int result = Message_Send(ptr);

	// result 반환값에 따라 상태 체킹
	switch (result)
	{
	case ERROR_DISCONNECTED:
		err_display("connect end");
	case DISCONNECTED:
		ptr->state = DISCONNECTED_STATE;
		return DISCONNECTED;

	case SOCKET_FALSE:
		return SOCKET_FALSE;

	case SOCKET_TRUE:
		return SOCKET_TRUE;
	}
}

// Packing
int Packing(char* buf, PROTOCOL protocol, const char* str1)
{
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

int Packing(char* buf, PROTOCOL protocol, SECOND_PROTOCOL second_protocol, const char* str1)
{
	int size = 0;
	char* ptr = buf + sizeof(int);
	int len = strlen(str1);

	ptr += memcpy_send(ptr, &protocol, sizeof(PROTOCOL), &size);
	ptr += memcpy_send(ptr, &second_protocol, sizeof(SECOND_PROTOCOL), &size);

	ptr += memcpy_send(ptr, &len, sizeof(int), &size);
	ptr += memcpy_send(ptr, str1, len, &size);

	ptr = buf;
	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}

int Packing(char* buf, PROTOCOL protocol, RESULT result, const char* str1)
{
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

int Packing(char* buf, PROTOCOL protocol, const char* str1, const char* str2)
{
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

int Packing(char* buf, PROTOCOL protocol, SECOND_PROTOCOL second_protocol, const char* str1, const char* str2)
{
	int size = 0;
	char* ptr = buf + sizeof(int);
	int len = strlen(str1);
	int len2 = strlen(str2);

	ptr += memcpy_send(ptr, &protocol, sizeof(PROTOCOL), &size);
	ptr += memcpy_send(ptr, &second_protocol, sizeof(SECOND_PROTOCOL), &size);

	ptr += memcpy_send(ptr, &len, sizeof(int), &size);
	ptr += memcpy_send(ptr, str1, len, &size);
	ptr += memcpy_send(ptr, &len2, sizeof(int), &size);
	ptr += memcpy_send(ptr, str2, len2, &size);

	ptr = buf;
	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}

// UnPacking
void UnPacking(const char* buf, char* str)
{
	const char* ptr = buf + sizeof(PROTOCOL);
	int len = 0;

	ptr += memcpy_recv(&len, ptr, sizeof(int));
	ptr += memcpy_recv(str, ptr, len);
}

void UnPacking(const char* buf, char* str, char* str2)
{
	const char* ptr = buf + sizeof(PROTOCOL);
	int len = 0;
	int len2 = 0;

	ptr += memcpy_recv(&len, ptr, sizeof(int));
	ptr += memcpy_recv(str, ptr, len);
	ptr += memcpy_recv(&len2, ptr, sizeof(int));
	ptr += memcpy_recv(str2, ptr, len2);
}
