#include "MyFunction.h"

ClientInfo* Client_Array[WSA_MAXIMUM_WAIT_EVENTS];
WSAEVENT Event_Array[WSA_MAXIMUM_WAIT_EVENTS];
int Total = 0;

UserInfo User_Array[100];
int User_Count = 0;

CRITICAL_SECTION cs;


bool Add_ClientInfo(SOCKET sock, SOCKADDR_IN addr);
void Remove_ClientInfo(ClientInfo* ptr);


int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	InitializeCriticalSection(&cs);

	int retval;
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR*)&addr, sizeof(addr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	Add_ClientInfo(listen_sock, addr);
	retval = WSAEventSelect(listen_sock, Event_Array[Total - 1], FD_ACCEPT | FD_CLOSE);
	if (retval == SOCKET_ERROR) err_quit("WSAEventSelect()");

	WSANETWORKEVENTS NetWorkEvent;
	SOCKET client_sock;
	SOCKADDR_IN client_addr;
	int index, addrlen, size;

	while (1)
	{
		index = WSAWaitForMultipleEvents(Total, Event_Array, FALSE, WSA_INFINITE, FALSE);
		if (index == WSA_WAIT_FAILED) continue;
		index -= WSA_WAIT_EVENT_0;

		retval = WSAEnumNetworkEvents(Client_Array[index]->sock, Event_Array[index], &NetWorkEvent);
		if (retval == SOCKET_ERROR) continue;

		if (NetWorkEvent.lNetworkEvents & FD_ACCEPT)
		{
			if (NetWorkEvent.iErrorCode[FD_ACCEPT_BIT] != 0)
			{
				err_display(NetWorkEvent.iErrorCode[FD_ACCEPT_BIT]);
				continue;
			}

			addrlen = sizeof(client_addr);
			client_sock = accept(Client_Array[index]->sock, (SOCKADDR*)&client_addr, &addrlen);
			if (client_sock == INVALID_SOCKET)
			{
				err_display("accept()");
				continue;
			}

			if (Total >= WSA_MAXIMUM_WAIT_EVENTS)
			{
				printf("[Error] 더 이상 접속을 받아들일 수 없습니다.\n");
				closesocket(client_sock);
				continue;
			}

			printf("\n[TCP 서버] 클라이언트 입장 : IP = %s, 포트번호 = %d\n",
				inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

			Add_ClientInfo(client_sock, client_addr);
			retval = WSAEventSelect(client_sock, Event_Array[Total - 1], FD_READ | FD_CLOSE);
			if (retval == SOCKET_ERROR) err_quit("WSAEventSelect()");
		}

		if (NetWorkEvent.lNetworkEvents & FD_READ)
		{
			if (NetWorkEvent.iErrorCode[FD_READ_BIT] != 0)
			{
				err_display(NetWorkEvent.iErrorCode[FD_READ_BIT]);
				continue;
			}

			ClientInfo* ptr = Client_Array[index];

			while (!ptr->recvflag)
			{
				int result = Packing_Recv(ptr);

				switch (result)
				{
				case SOCKET_FALSE:
					continue;

				case DISCONNECTED:
					ptr->state = DISCONNECTED_STATE;
				case SOCKET_TRUE:
					ptr->recvflag = true;
					break;
				}
			}

			ptr->recvflag = false;
			PROTOCOL protocol = GetProtocol(ptr->recv_buf);

			switch (ptr->state)
			{
			case INTRO_MENU_SELECT_STATE:
				switch (protocol)
				{
				case JOIN:
					size = Packing(ptr->send_buf, ID_PW_INFO, ID_MSG, PW_MSG);
					retval = send(ptr->sock, ptr->send_buf, size, 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}

					ptr->state = JOIN_RESULT_STATE;
					break;

				case LOGIN:
					size = Packing(ptr->send_buf, ID_PW_INFO, ID_MSG, PW_MSG);
					retval = send(ptr->sock, ptr->send_buf, size, 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}

					ptr->state = LOGIN_RESULT_STATE;
					break;

				case EXIT:
					ptr->state = DISCONNECTED_STATE;
					break;
				}
				break;

			case JOIN_RESULT_STATE:
			{
				UnPacking(ptr->recv_buf, ptr->temp_user.id, ptr->temp_user.pw);
				RESULT result = NODATA;

				for (int i = 0; i < User_Count; i++)
				{
					if (!strcmp(User_Array[i].id, ptr->temp_user.id))
					{
						result = ERROR_JOIN_EXISTS;
						size = Packing(ptr->send_buf, JOIN_RESULT, result, ERROR_JOIN_EXISTS_MSG);
						break;
					}
				}

				if (result == NODATA)
				{
					EnterCriticalSection(&cs);
					memcpy(&User_Array[User_Count], &ptr->temp_user, sizeof(UserInfo));
					++User_Count;
					LeaveCriticalSection(&cs);

					result = JOIN_SUCCESS;
					size = Packing(ptr->send_buf, JOIN_RESULT, result, JOIN_SUCCESS_MSG);
				}

				retval = send(ptr->sock, ptr->send_buf, size, 0);
				if (retval == SOCKET_ERROR)
				{
					err_display("send()");
					break;
				}

				ptr->state = INTRO_STATE;
			}
				break;

			case LOGIN_RESULT_STATE:
			{
				UnPacking(ptr->recv_buf, ptr->temp_user.id, ptr->temp_user.pw);
				RESULT result = NODATA;

				for (int i = 0; i < User_Count; i++)
				{
					if (!strcmp(User_Array[i].id, ptr->temp_user.id))
					{
						if (!strcmp(User_Array[i].pw, ptr->temp_user.pw))
						{
							if (!User_Array[i].login_state)
							{
								User_Array[i].login_state = true;
								ptr->user = User_Array[i];
								result = LOGIN_SUCCESS;
								size = Packing(ptr->send_buf, LOGIN_RESULT, result, LOGIN_SUCCESS_MSG);

								ptr->state = LOGIN_STATE;

							}
							else
							{
								result = ERROR_LOGIN_EXISTS;
								size = Packing(ptr->send_buf, LOGIN_RESULT, result, ERROR_LOGIN_EXISTS_MSG);

								ptr->state = INTRO_STATE;
							}
						}
						else
						{
							result = ERROR_LOGIN_PW;
							size = Packing(ptr->send_buf, LOGIN_RESULT, result, ERROR_LOGIN_PW_MSG);

							ptr->state = INTRO_STATE;
						}
						break;
					}
				}

				if (result == NODATA)
				{
					result = ERROR_LOGIN_ID;
					size = Packing(ptr->send_buf, LOGIN_RESULT, result, ERROR_LOGIN_ID_MSG);

					ptr->state = INTRO_STATE;
				}

				retval = send(ptr->sock, ptr->send_buf, size, 0);
				if (retval == SOCKET_ERROR)
				{
					err_display("send()");
					break;
				}
			}
				break;

			case LOGIN_MENU_SELECT_STATE:
				switch (protocol)
				{
				case LOGOUT:
					for (int i = 0; i < User_Count; i++)
					{
						if (!strcmp(User_Array[i].id, ptr->user.id))
						{
							User_Array[i].login_state = false;
							break;
						}
					}

					size = Packing(ptr->send_buf, LOGOUT_RESULT, LOGOUT_MSG);
					retval = send(ptr->sock, ptr->send_buf, size, 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}

					ptr->state = INTRO_STATE;
					break;

				case REMOVE:
					for (int i = 0; i < User_Count; i++)
					{
						if (!strcmp(User_Array[i].id, ptr->user.id))
						{
							for (int j = i; j < User_Count - 1; j++)
							{
								User_Array[j] = User_Array[j + 1];
							}
							ZeroMemory(&User_Array[User_Count - 1], sizeof(UserInfo));
							User_Count--;
							break;
						}
					}

					size = Packing(ptr->send_buf, REMOVE_RESULT, REMOVE_MSG);
					retval = send(ptr->sock, ptr->send_buf, size, 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}

					ptr->state = INTRO_STATE;
					break;
				}
				break;
			}

			// Menu Send
			switch (ptr->state)
			{
			case INTRO_STATE:
				size = Packing(ptr->send_buf, INTRO_MENU, INTRO_MENU_MSG);
				retval = send(ptr->sock, ptr->send_buf, size, 0);
				if (retval == SOCKET_ERROR)
				{
					err_display("send()");
					break;
				}

				ptr->state = INTRO_MENU_SELECT_STATE;
				break;

			case LOGIN_STATE:
				size = Packing(ptr->send_buf, LOGIN_MENU, LOGIN_MENU_MSG);
				retval = send(ptr->sock, ptr->send_buf, size, 0);
				if (retval == SOCKET_ERROR)
				{
					err_display("send()");
					break;
				}

				ptr->state = LOGIN_MENU_SELECT_STATE;
				break;
			}
		}

		if (NetWorkEvent.lNetworkEvents & FD_CLOSE)
		{
			if (NetWorkEvent.iErrorCode[FD_CLOSE_BIT] != 0) err_display(NetWorkEvent.iErrorCode[FD_CLOSE_BIT]);
			
			for (int i = 0; i < User_Count; i++)
			{
				if (!strcmp(User_Array[i].id, Client_Array[index]->user.id))
				{
					User_Array[i].login_state = false;
					break;
				}
			}
				
			Remove_ClientInfo(Client_Array[index]);
		}
	}

	DeleteCriticalSection(&cs);
	WSACleanup();
	return 0;
}

bool Add_ClientInfo(SOCKET sock, SOCKADDR_IN addr)
{
	EnterCriticalSection(&cs);
	ClientInfo* ptr = new ClientInfo();
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

	ptr->sock = sock;
	memcpy(&ptr->addr, &addr, sizeof(addr));
	ptr->comp_recv_bytes = ptr->comp_send_bytes = 0;
	ptr->recv_bytes = ptr->send_bytes = 0;

	ptr->state = INTRO_STATE;

	ptr->sizeflag = false;
	ptr->recvflag = false;

	ZeroMemory(&ptr->user, sizeof(ptr->user));
	ZeroMemory(&ptr->temp_user, sizeof(ptr->temp_user));

	Client_Array[Total] = ptr;
	Event_Array[Total] = hEvent;
	++Total;

	LeaveCriticalSection(&cs);
	return true;
}

void Remove_ClientInfo(ClientInfo* ptr)
{
	EnterCriticalSection(&cs);

	for (int i = 0; i < Total; i++)
	{
		if (ptr->sock == Client_Array[i]->sock)
		{
			printf("\n[TCP 서버] 클라이언트 종료 : IP = %s, 포트번호 = %d\n",
				inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));

			closesocket(ptr->sock);
			WSACloseEvent(Event_Array[i]);
			delete Client_Array[i];

			for (int j = i; j < Total - 1; j++)
			{
				Client_Array[j] = Client_Array[j + 1];
				Event_Array[j] = Event_Array[j + 1];
			}

			Client_Array[Total - 1] = nullptr;
			--Total;
			LeaveCriticalSection(&cs);
			return;
		}
	}

	LeaveCriticalSection(&cs);
}