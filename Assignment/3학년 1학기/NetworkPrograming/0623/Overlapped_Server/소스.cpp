#include "MyFunction.h"

_ClientInfo* ClientArray[WSA_MAXIMUM_WAIT_EVENTS];
WSAEVENT EventArray[WSA_MAXIMUM_WAIT_EVENTS];
int Total_Count;

_UserInfo UserArray[100];
int User_Count;

CRITICAL_SECTION cs;

DWORD WINAPI WorkerThread(LPVOID arg);

bool Add_ClientInfo(SOCKET sock, SOCKADDR_IN addr);
void Remove_ClientInfo(_ClientInfo* ptr);

int main()
{
	int retval;
	InitializeCriticalSection(&cs);

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(PORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	WSAEVENT hEvent = WSACreateEvent();
	if (hEvent == WSA_INVALID_EVENT)
		err_quit("WSACreateEvent()");
	EventArray[Total_Count++] = hEvent;

	HANDLE hThread = CreateThread(nullptr, 0, WorkerThread, nullptr, 0, nullptr);
	if (hThread == nullptr) return 1;
	CloseHandle(hThread);

	SOCKET client_sock;
	SOCKADDR_IN client_addr;
	int addrlen;

	while (1)
	{
		addrlen = sizeof(client_addr);
		client_sock = accept(listen_sock, (SOCKADDR*)&client_addr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}

		printf("\n[TCP 서버] 클라이언트 접속 : IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

		if (!Add_ClientInfo(client_sock, client_addr))
		{
			closesocket(client_sock);
			printf("\n[TCP 서버] 클라이언트 종료 : IP 주소=%s, 포트 번호=%d\n",
				inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

			continue;
		}

		_ClientInfo* ptr = ClientArray[Total_Count - 1];

		retval = WSARecv(ptr->sock, &ptr->wsabuf, 1, &ptr->recvbytes, &ptr->flag, &ptr->overlapped, nullptr);
		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				err_display("WSARecv()");
				Remove_ClientInfo(ptr);
				continue;
			}
		}
		ptr->recv_bytes = ptr->recvbytes;
		ptr->recvflag = true;

		WSASetEvent(EventArray[0]);
	}

	closesocket(listen_sock);
	DeleteCriticalSection(&cs);
	WSACleanup();

	return 0;
}

DWORD WINAPI WorkerThread(LPVOID arg)
{
	int retval;
	char msg[MSGSIZE];
	char buf[BUFSIZE];

	while (1)
	{
		DWORD index = WSAWaitForMultipleEvents(Total_Count, EventArray, false, WSA_INFINITE, false);
		if (index == WSA_WAIT_FAILED) continue;

		index -= WSA_WAIT_EVENT_0;
		WSAResetEvent(EventArray[index]);

		if (index == 0) continue;

		_ClientInfo* ptr = ClientArray[index];

		retval = WSAGetOverlappedResult(ptr->sock, &ptr->overlapped, &ptr->comp_bytes, false, &ptr->flag);
		if (retval == false)
		{
			Remove_ClientInfo(ptr);
			continue;
		}

		ptr->comp_recvbytes += ptr->comp_bytes;

		if (ptr->comp_recvbytes == sizeof(int) && !ptr->recvflag)
		{
			ptr->comp_recvbytes = 0;
			memcpy(&ptr->recv_bytes, ptr->recv_buf, sizeof(int));
			ptr->recvflag = true;
		}
		else if (ptr->comp_recvbytes == ptr->recv_bytes && ptr->recvflag)
		{
			switch (ptr->state)
			{
			case INTRO_STATE:
				ptr->send_bytes = Packing(ptr->send_buf, INTRO, INTRO_MSG);
				if (Message_Send(ptr))
				{
					ptr->state = INTRO_MENU_SELECT_STATE;
				}
				break;

			case INTRO_MENU_SELECT_STATE:
				switch (GetProtocol(ptr->recv_buf))
				{
				case JOIN:
					ptr->send_bytes = Packing(ptr->send_buf, ID_PW_INFO, ID_MSG, PW_MSG);
					Message_Send(ptr);

					ptr->state = JOIN_RESULT_STATE;
					break;

				case LOGIN:
					ptr->send_bytes = Packing(ptr->send_buf, ID_PW_INFO, ID_MSG, PW_MSG);
					Message_Send(ptr);

					ptr->state = LOGIN_RESULT_STATE;
					break;

				case EXIT:
					ptr->state = DISCONNECTED_STATE;
					break;
				}

				break;

			case JOIN_RESULT_STATE:
			{
				ZeroMemory(&ptr->temp_user, sizeof(ptr->temp_user));
				UnPacking(ptr->recv_buf, ptr->temp_user.id, ptr->temp_user.pw);
				RESULT result = NODATA;

				EnterCriticalSection(&cs);
				for (int i = 0; i < User_Count; i++)
				{
					if (!strcmp(UserArray[i].id, ptr->temp_user.id))
					{
						result = ERROR_JOIN_EXISTS;
						ptr->send_bytes = Packing(ptr->send_buf, JOIN_RESULT, result, ERROR_JOIN_MSG);
						break;
					}
				}

				if (result == NODATA)
				{
					EnterCriticalSection(&cs);
					UserArray[User_Count] = ptr->temp_user;
					++User_Count;
					LeaveCriticalSection(&cs);

					result = JOIN_SUCCESS;
					ptr->send_bytes = Packing(ptr->send_buf, JOIN_RESULT, result, JOIN_SUCCESS_MSG);
				}
				LeaveCriticalSection(&cs);

				Message_Send(ptr);

				ptr->state = INTRO_MENU_SELECT_STATE;

				ptr->send_bytes = Packing(ptr->send_buf, INTRO, INTRO_MSG);
				if (Message_Send(ptr))
				{
					ptr->state = INTRO_MENU_SELECT_STATE;
				}
			}
				break;

			case LOGIN_RESULT_STATE:
			{
				ZeroMemory(&ptr->temp_user, sizeof(ptr->temp_user));
				UnPacking(ptr->recv_buf, ptr->temp_user.id, ptr->temp_user.pw);
				RESULT result = NODATA;

				EnterCriticalSection(&cs);
				for (int i = 0; i < User_Count; i++)
				{
					if (!strcmp(UserArray[i].id, ptr->temp_user.id))
					{
						if (!strcmp(UserArray[i].pw, ptr->temp_user.pw))
						{
							if (!UserArray[i].state)
							{
								//UserArray[i].state = true;
								ptr->user = UserArray[i];
								result = LOGIN_SUCCESS;
								ptr->send_bytes = Packing(ptr->send_buf, LOGIN_RESULT, result, LOGIN_SUCCESS_MSG);

								//ptr->state = LOGIN_STATE;
							}
							else
							{
								result = ERROR_LOGIN_EXISTS;
								ptr->send_bytes = Packing(ptr->send_buf, LOGIN_RESULT, result, ERROR_LOGIN_EXISTS_MSG);
							}
						}
						else
						{
							result = ERROR_LOGIN_PW;
							ptr->send_bytes = Packing(ptr->send_buf, LOGIN_RESULT, result, ERROR_LOGIN_PW_MSG);
						}
					}
				}
				LeaveCriticalSection(&cs);

				if (result == NODATA)
				{
					result = ERROR_LOGIN_ID;
					ptr->send_bytes = Packing(ptr->send_buf, LOGIN_RESULT, result, ERROR_LOGIN_ID_MSG);
				}

				Message_Send(ptr);
				ZeroMemory(&ptr->user, sizeof(ptr->user));

				ptr->send_bytes = Packing(ptr->send_buf, INTRO, INTRO_MSG);
				if (Message_Send(ptr))
				{
					ptr->state = INTRO_MENU_SELECT_STATE;
				}
			}
				break;
			}

			ptr->comp_recvbytes = 0;
			ptr->recv_bytes = sizeof(int);
			ptr->recvflag = false;
		}

		if (ptr->state == DISCONNECTED_STATE)
		{
			Remove_ClientInfo(ptr);
			continue;
		}
		else
		{
			ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
			ptr->overlapped.hEvent = EventArray[index];

			Message_Recv(ptr);
		}
	}
	return 0;
}

bool Add_ClientInfo(SOCKET sock, SOCKADDR_IN addr)
{
	EnterCriticalSection(&cs);
	_ClientInfo* ptr = new _ClientInfo();

	if (ptr == nullptr)
	{
		printf("[Error] 메모리가 부족합니다.\n");
		LeaveCriticalSection(&cs);
		return false;
	}

	WSAEVENT hEvent = WSACreateEvent();
	if (hEvent == WSA_INVALID_EVENT)
	{
		err_display("WSACreateEvent()");
		LeaveCriticalSection(&cs);
		return false;
	}

	ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
	ptr->overlapped.hEvent = hEvent;

	ptr->sock = sock;
	memcpy(&ptr->addr, &addr, sizeof(addr));

	ZeroMemory(&ptr->wsabuf, sizeof(ptr->wsabuf));
	ptr->wsabuf.buf = ptr->recv_buf;
	ptr->wsabuf.len = BUFSIZE;

	ptr->flag = 0;
	ptr->recvbytes = 0;
	ptr->sendbytes = 0;

	ptr->comp_bytes = 0;
	ptr->comp_recvbytes = ptr->comp_sendbytes = 0;
	ptr->recv_bytes = ptr->send_bytes = 0;

	ptr->state = INTRO_STATE;

	ptr->sizeflag = false;
	ptr->recvflag = false;

	ZeroMemory(&ptr->user, sizeof(ptr->user));
	ZeroMemory(&ptr->temp_user, sizeof(ptr->temp_user));

	ClientArray[Total_Count] = ptr;
	EventArray[Total_Count] = hEvent;
	++Total_Count;

	LeaveCriticalSection(&cs);
	return true;
}

void Remove_ClientInfo(_ClientInfo* ptr)
{
	EnterCriticalSection(&cs);

	for (int i = 1; i < Total_Count; i++)
	{
		if (ptr->sock == ClientArray[i]->sock)
		{
			printf("\n[TCP 서버] 클라이언트 종료 : IP = %s, 포트번호 = %d\n",
				inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));

			closesocket(ptr->sock);
			delete ClientArray[i];
			WSACloseEvent(EventArray[i]);

			for (int j = i; j < Total_Count - 1; j++)
			{
				ClientArray[j] = ClientArray[j + 1];
				EventArray[j] = EventArray[j + 1];
			}

			ClientArray[Total_Count - 1] = nullptr;
			EventArray[Total_Count - 1] = nullptr;
			--Total_Count;

			LeaveCriticalSection(&cs);
			return;
		}
	}

	LeaveCriticalSection(&cs);
}
