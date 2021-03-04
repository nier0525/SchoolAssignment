#include "MyFunction.h"

// ���� �ʱ�ȭ �Լ�
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

// ���� �Լ� ���� ��� �� ����
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

// ���� �Լ� ���� ���
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

// �������� ����
PROTOCOL GetProtocol(char* buf) 
{
	PROTOCOL protocol;
	memcpy(&protocol, buf, sizeof(PROTOCOL));

	return protocol;
}

// ���� ���� ��ŷ �Լ�
int memcpy_send(void* ptr, const void* dst, size_t size, int* len)
{
	memcpy(ptr, dst, size);	// �޸� ����
	*len += size;						// �޸��� ũ�⸦ ���ڰ����� ���� ũ�⸸ŭ ����
	return size;						// ���ڰ����� ���� ũ�� ����
}

// ���� ���� ����ŷ �Լ�
int memcpy_recv(void* dst, const void* ptr, size_t size) 
{
	memcpy(dst, ptr, size);	// �޸� ����
	return size;						// ���ڰ����� ���� ũ�� ����
}

// �� ���ŷ ���� �Լ�
int Message_Recv(Client_Info* ptr)
{
	// �� ���� �� �޴� �ٴ� ������ ���� ������ ����� �޴� �۾�
	// �̹� ���� �������� ũ�⸦ �����ϰ� �޴´�.
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
		// ���� ���� ���� ũ�⸸ŭ �����ش�.
		ptr->total_recvbytes += retval;
		// ���� ���ۿ��� ��� �����͸� �ҷ����� ���
		if (ptr->total_recvbytes == ptr->recvbytes)
		{
			ptr->total_recvbytes = ptr->recvbytes = 0;
			return SOCKET_TRUE;
		}
		// ���� ���� ���ۿ� �����Ͱ� ���� �ִ� ���
		else
		{
			return SOCKET_FALSE;
		}
	}
}

// �� ���ŷ �۽� �Լ�
int Message_Send(Client_Info* ptr)
{
	// �۽� ���� ������ ��찡 �и��� �ְ�, �� ���� �����ٴ� ������ ����.
	// �̹� ���� �۽� �������� ũ�⸦ �����ϰ� �ٽ� ������.
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
		// ���� �۽� �����͸�ŭ ���Ѵ�.
		ptr->total_sendbytes += retval;

		// �۽� �� �����͸� ��� ���� ���
		if (ptr->sendbytes == ptr->total_sendbytes)
		{
			ptr->total_sendbytes = ptr->sendbytes = 0;
			return SOCKET_TRUE;
		}
		// ���� �۽� �� �����Ͱ� ���� ���
		else
		{
			return SOCKET_FALSE;
		}
	}
}

// ���� ���� ����
int Packing_Recv(Client_Info* ptr)
{
	// ���� ���� ��Ŷ�� ��ü ũ�⸦ ���� �޴´�.
	if (!ptr->recv_flag)
	{
		// ��Ŷ�� ���� �տ� ��ġ�� ��ü ũ��� int ���̱� ������ �� int �� ��ŭ�� �޴´�.
		ptr->recvbytes = sizeof(int);
		int retval = Message_Recv(ptr);

		switch (retval)
		{
		// ��Ŷ�� ��ü ũ�� = ������ ������ ũ�� ������ �ٷ� �������ش�.
		// ��ü ũ�⸦ �˾Ƴ����� recv_flag �� true �� �ٲ��ش�.
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

	// ���� ������ ��ü ũ�⸦ ���� ��
	else
	{
		// �˾� �� ���� ������ ��ü ũ�⸸ŭ �����Ѵ�.
		int retval = Message_Recv(ptr);

		switch (retval)
		{
		// ���� �����͸� ��� �޾Ƴ����� �ٽ� recv_flag �� false �� �ʱ�ȭ ���ش�.
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

// �۽� ���� üũ
int Result_Send(Client_Info* ptr)
{
	int result = Message_Send(ptr);

	// result ��ȯ���� ���� ���� üŷ
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
