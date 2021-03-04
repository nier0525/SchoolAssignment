#include "MyFunction.h"

LPClientInfo ClientArray[WSA_MAXIMUM_WAIT_EVENTS];
int TotalCount;

UserInfo UserArray[10];
int UserCount;

CRITICAL_SECTION cs;

DWORD WINAPI WorkerThread(void* arg);

bool Add_Client(SOCKET sock, SOCKADDR_IN addr);
void Remove_Client(LPClientInfo ptr);

int main()
{
	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return -1;

	// 입출력 완료 포트 생성
	HANDLE hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (hcp == NULL) return 1;

	// CPU 개수 확인
	SYSTEM_INFO sys;
	GetSystemInfo(&sys);

	// CPU * 2 개의 작업자 스레드 생성
	HANDLE hThread;
	for (int i = 0; i < (int)sys.dwNumberOfProcessors * 2; i++)
	{
		hThread = CreateThread(nullptr, 0, WorkerThread, hcp, 0, nullptr);
		if (hThread == NULL) return 1;
		CloseHandle(hThread);
	}

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("listen socket");

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(PORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	InitializeCriticalSection(&cs);

	SOCKET sock;
	SOCKADDR_IN addr;
	int addrlen;
	DWORD recvbytes, flags;

	while (1)
	{
		addrlen = sizeof(addr);
		sock = accept(listen_sock, (SOCKADDR*)&addr, &addrlen);
		if (sock == INVALID_SOCKET)
		{
			err_display("accept");
			break;
		}

		printf("\n[TCP 서버] 클라이언트 접속 : IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

		CreateIoCompletionPort((HANDLE)sock, hcp, (DWORD)sock, 0);

		if (!Add_Client(sock, addr))
		{
			closesocket(sock);
			printf("\n[TCP 서버] 클라이언트 종료 : IP 주소=%s, 포트 번호=%d\n",
				inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
			continue;
		}

		LPClientInfo ptr = ClientArray[TotalCount - 1];

		if (!Recv(ptr))
			ptr->state = DISCONNECTED_STATE;
	}

	DeleteCriticalSection(&cs);
	closesocket(listen_sock);
	WSACleanup();

	return 0;
}

DWORD WINAPI WorkerThread(void* arg)
{
	int retval;
	HANDLE hcp = (HANDLE)arg;

	while (1)
	{
		DWORD cbTransferred;
		SOCKET sock;
		LPClientInfo ptr;
		LPWSAOVERLAPPED_EX overlapped;

		retval = GetQueuedCompletionStatus(hcp, &cbTransferred, (LPDWORD)&sock, (LPOVERLAPPED*)&overlapped, INFINITE);
		ptr = overlapped->ptr;

		if (retval == FALSE)
		{
			DWORD temp1, temp2;
			WSAGetOverlappedResult(ptr->sock, &overlapped->overlapped, &temp1, false, &temp2);
			err_display("WSAGetOverlappedResult");

			ptr->state = DISCONNECTED_STATE;
		}
		else if (cbTransferred == 0)
		{
			ptr->state = DISCONNECTED_STATE;
		}
		
		switch (ptr->state)
		{
		case MENU_SELECT_STATE:
		{
			int result = CompleteRecv(ptr, cbTransferred);

			switch (result)
			{
			case SOC_ERROR:
				ptr->state = DISCONNECTED_STATE;
				break;

			case SOC_FALSE:
				continue;

			case SOC_TRUE:
				break;
			}

			switch (GetProtocol(ptr->recv_buf))
			{
			case JOIN_INFO:
				ZeroMemory(&ptr->user, sizeof(ptr->user));
				UnPacking(ptr->recv_buf, ptr->user.id, ptr->user.pw);
				ptr->state = JOIN_STATE;
				break;

			case LOGIN_INFO:
				ZeroMemory(&ptr->user, sizeof(ptr->user));
				UnPacking(ptr->recv_buf, ptr->user.id, ptr->user.pw);
				ptr->state = LOGIN_STATE;
				break;
			}
		}
		break;

		case RESULT_SEND_STATE:
		{
			int result = CompleteSend(ptr, cbTransferred);

			switch (result)
			{
			case SOC_ERROR:
				ptr->state = DISCONNECTED_STATE;
				break;

			case SOC_FALSE:
				continue;

			case SOC_TRUE:
				break;
			}

			ptr->state = MENU_SELECT_STATE;

			Recv(ptr);

			continue;
		}
		break;

		}

		switch (ptr->state)
		{
		case JOIN_STATE:
		{
			RESULT result = NODATA;
			int size;

			for (int i = 0; i < UserCount; i++)
			{
				if (!strcmp(UserArray[i].id, ptr->user.id))
				{
					result = ERROR_JOIN_EXISTS;
					size = Packing(ptr->send_buf, JOIN_RESULT, result, ERROR_JOIN_MSG);
					break;
				}
			}

			if (result == NODATA)
			{
				EnterCriticalSection(&cs);
				UserArray[UserCount++] = ptr->user;
				LeaveCriticalSection(&cs);

				result = JOIN_SUCCESS;
				size = Packing(ptr->send_buf, JOIN_RESULT, result, JOIN_SUCCESS_MSG);
			}

			if (!Send(ptr, size))
				ptr->state = DISCONNECTED_STATE;
			else
				ptr->state = RESULT_SEND_STATE;			
		}
		break;

		case LOGIN_STATE:
		{
			RESULT result = NODATA;
			int size;

			EnterCriticalSection(&cs);
			for (int i = 0; i < UserCount; i++)
			{
				if (!strcmp(UserArray[i].id, ptr->user.id))
				{
					if (!strcmp(UserArray[i].pw, ptr->user.pw))
					{
						if (!UserArray[i].state)
						{
							result = LOGIN_SUCCESS;
							size = Packing(ptr->send_buf, LOGIN_RESULT, result, LOGIN_SUCCESS_MSG);
						}
						else
						{
							result = ERROR_LOGIN_EXISTS;
							size = Packing(ptr->send_buf, LOGIN_RESULT, result, ERROR_LOGIN_EXISTS_MSG);
						}
					}
					else
					{
						result = ERROR_LOGIN_PW;
						size = Packing(ptr->send_buf, LOGIN_RESULT, result, ERROR_LOGIN_PW_MSG);
					}
					break;
				}
			}
			LeaveCriticalSection(&cs);

			if (result == NODATA)
			{
				result = ERROR_LOGIN_ID;
				size = Packing(ptr->send_buf, LOGIN_RESULT, result, ERROR_LOGIN_ID_MSG);
			}

			if (!Send(ptr, size))
				ptr->state = DISCONNECTED_STATE;
			else
				ptr->state = RESULT_SEND_STATE;
		}
		break;
		}

		if (ptr->state == DISCONNECTED_STATE)
		{
			Remove_Client(ptr);
			continue;
		}
	}
}

bool Add_Client(SOCKET sock, SOCKADDR_IN addr)
{
	LPClientInfo ptr = new ClientInfo();

	if (ptr == nullptr || TotalCount >= WSA_MAXIMUM_WAIT_EVENTS)
	{
		return false;
	}

	EnterCriticalSection(&cs);

	ptr->sock = sock;
	memcpy(&ptr->addr, &addr, sizeof(addr));

	ptr->r_overlapped.ptr = ptr;
	ptr->r_overlapped.type = IO_RECV;

	ptr->s_overlapped.ptr = ptr;
	ptr->s_overlapped.type = IO_SEND;

	ptr->r_sizeflag = false;
	ptr->state = MENU_SELECT_STATE;

	ptr->recvbytes = ptr->sendbytes = 0;
	ptr->comp_recvbytes = ptr->comp_sendbytes = 0;

	ZeroMemory(&ptr->user, sizeof(ptr->user));

	ClientArray[TotalCount] = ptr;
	++TotalCount;

	LeaveCriticalSection(&cs);
	return true;
}

void Remove_Client(LPClientInfo ptr)
{
	EnterCriticalSection(&cs);

	for (int i = 0; i < TotalCount; i++)
	{
		if (ptr->sock == ClientArray[i]->sock)
		{
			printf("\n[TCP 서버] 클라이언트 종료 : IP 주소=%s, 포트 번호=%d\n",
				inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));

			closesocket(ptr->sock);
			delete ClientArray[i];

			for (int j = i; j < TotalCount - 1; j++)
			{
				ClientArray[j] = ClientArray[j + 1];
			}

			ClientArray[TotalCount - 1] = nullptr;
			--TotalCount;

			break;
		}
	}

	LeaveCriticalSection(&cs);
}