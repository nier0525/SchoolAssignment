#include "MyFunction.h"

// ���� ����
// Ŭ���̾�Ʈ ���� ����Ʈ
Client_Info* Client_Array[FD_SETSIZE];
int Client_Count = 0;

// ���� ���� ����Ʈ
User_Info userlist[USERSIZE];
int User_Count = 0;

// ��� ���� ����Ʈ
Auction_Info Auction_List[5];
int Auction_Count = 0;


// �� ���ŷ ���� ��� �Լ�
void Recv_PackingProcess(Client_Info* ptr);

// �� ���ŷ �۽� ��� �Լ�
void Send_PackingProcess(Client_Info* ptr);

// Ŭ���̾�Ʈ ���� �߰�
bool Add_ClientInfo(SOCKET sock, SOCKADDR_IN addr);

// Ŭ���̾�Ʈ ���� ����
void Remove_ClientInfo(Client_Info* ptr, int& index);

// �޴� ����
void MenuSelectProcess(Client_Info* ptr, PROTOCOL protocol);

// ȸ�� ����
void JoinProcess(Client_Info* ptr);

// �α���
void LoginProcess(Client_Info* ptr);

// ��� ����Ʈ
void AuctionIntroProcess(Client_Info* ptr);

// ���
void AuctionProcess(Client_Info* ptr);

// ��� ���
void AuctionResultProcess(int index);

// ������ ���� ��� ��ǰ ���
void WareViewProcess(Client_Info* ptr);

// �α׾ƿ�
void LogoutProcess(Client_Info* ptr);

// ȸ��Ż��
void RemoveProcess(Client_Info* ptr);

int main()
{
	// WSA Init
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	// listen_sock and listen_addr Init
	SOCKET listen_sock;
	SOCKADDR_IN listen_addr;

	ListenSocket_Init(listen_sock, listen_addr);

	// ������ ��ſ� ���� ����
	FD_SET rset, wset;

	SOCKET sock;
	SOCKADDR_IN addr;
	int retval, addrlen, index;

	// ������ �̸� ��� ����Ʈ �߰�
	Auction_List[Auction_Count++] = Auction_Info("����Ŭ ������ ��ȭ", 50000, 3, 0);
	Auction_List[Auction_Count++] = Auction_Info("�ں� ����̾�Ʈ�� ����", 40000, 2, 0);
	Auction_List[Auction_Count++] = Auction_Info("���ӽ� �ϵ��� ������", 30000, 2, 0);

	while (1)
	{
		// �б�, ���� �� �ʱ�ȭ
		FD_ZERO(&rset);
		FD_ZERO(&wset);
		// listen �� �б���� �׻� �ѵ�
		FD_SET(listen_sock, &rset);

		// Ŭ���̾�Ʈ rset, wset ����
		for (index = 0; index < Client_Count; index++)
		{
			// Ŭ���̾�Ʈ�� �б� ���� �׻� �ѵ�
			FD_SET(Client_Array[index]->sock, &rset);

			// ���¿� ���� Ŭ���̾�Ʈ�� ���� ���� Ŵ
			switch (Client_Array[index]->state)
			{
			case INTRO_STATE:
			case INFO_STATE:
			case NEW_STATE:
			case LOGIN_STATE:
			case LOGIN_INTRO_STATE:
			case AUCTION_INTRO_STATE:
			case AUCTION_DEPOSIT_STATE:
			case AUCTION_ERROR_STATE:
			case AUCTION_WAIT_MSG_STATE:
			case AUCTION_FAIL_STATE:
			case AUCTION_SUCCESS_STATE:
			case WARE_STATE:
			case MONEY_STATE:
			case DEPOSIT_STATE:
			case DEPOSIT_RESULT_STATE:				
			case LOGOUT_STATE:
			case REMOVE_STATE:
				FD_SET(Client_Array[index]->sock, &wset);
				break;
			}
		}

		// select()
		retval = select(0, &rset, &wset, NULL, NULL);
		if (retval == SOCKET_ERROR) err_quit("select()");

		// ���� �� �˻�(1) : Ŭ���̾�Ʈ ����
		if (FD_ISSET(listen_sock, &rset))
		{
			// listen Queue �� �ִ� Ŭ���̾�Ʈ ���� ����
			addrlen = sizeof(addr);
			sock = accept(listen_sock, (SOCKADDR*)&addr, &addrlen);
			if (sock == INVALID_SOCKET)	err_display("acccept()");		
			// Ŭ���̾�Ʈ ���� �߰�
			else	Add_ClientInfo(sock, addr);			
		}

		// ���� �� �˻�(2) : ������ ���
		for (index = 0; index < Client_Count; index++)
		{
			Client_Info* ptr = Client_Array[index];

			// ���� Ŭ���̾�Ʈ�� �б� ���� ���� �ִ� ���
			if (FD_ISSET(ptr->sock, &rset))
			{
				// ���� ���� ������ ���� �Լ�
				int result = Packing_Recv(ptr);

				// result ��ȯ ���� ���� �۾�
				switch (result)
				{
				case DISCONNECTED:
					ptr->state = DISCONNECTED_STATE;
					break;

				case SOCKET_FALSE:
					continue;

				case SOCKET_TRUE:
					break;
				}

				// �����͸� ��� �޾Ұų�, �߰��� Ŭ���̾�Ʈ�� �����ų� �� ��� �� ���� ������
				// ������ �����͸� ���� �۾�
				Recv_PackingProcess(ptr);
			}

			// ���� Ŭ���̾�Ʈ�� ���� ���� ���� �ִ� ���
			if (FD_ISSET(ptr->sock, &wset))
			{
				// �����͸� ��� �۽��ߴٸ� �۽� ���� �۾�
				if (ptr->sendbytes == 0) Send_PackingProcess(ptr);

				// result ���� ���� �����͸� ��� �۽��ߴ°�? �� �߰��� �������� ���ߴ°�? �� ��ȯ
				// �۽� ���¸� üŷ���ִ� �Լ� ( ���ο��� �۽����ָ鼭 ���÷� üŷ���� )
				int result = Result_Send(ptr);

				// �۽� �� �����Ͱ� �����ִٸ� continue
				if (result == SOCKET_FALSE) continue;
			}

			// ���� Ŭ���̾�Ʈ�� ���°� ���� ������ ���
			if (ptr->state == DISCONNECTED_STATE)
			{
				// Ŭ���̾�Ʈ ���� ���� �۾�
				Remove_ClientInfo(ptr, index);
				continue;
			}
		}
	}

	// listen Sock Close
	closesocket(listen_sock);

	// WSA Cleanup
	WSACleanup();
	return 0;
}

// �� ���ŷ ���� ��� �Լ�
void Recv_PackingProcess(Client_Info* ptr)
{
	// �������� �и�
	PROTOCOL protocol = GetProtocol(ptr->recv_buf);

	// ���¿� ���� �з�
	switch (ptr->state)
	{
	
	// �޴� �Է� ����
	case MENU_SELECT_STATE:
		// �޴� ���� �Լ� ����
		MenuSelectProcess(ptr, protocol);
		break;

	// ������ ID,PW ���� �� ����
	case USER_INFO_STATE:
		ZeroMemory(&ptr->user, sizeof(ptr->user));
		UnPacking(ptr->recv_buf, ptr->user.id, ptr->user.pw);

		// ȸ������, �α��� �������ݿ� ���� ���� ����
		switch (protocol)
		{
		case NEW_INFO:
			ptr->state = NEW_STATE;
			break;

		case LOGIN_INFO:
			ptr->state = LOGIN_STATE;
			break;
		}
		break;

	// �Ա� ��û ���� �� ����
	case DEPOSIT_INFO_STATE:
		ZeroMemory(ptr->msg, sizeof(ptr->msg));
		UnPacking(ptr->recv_buf, ptr->msg);

		// Ŭ���̾�Ʈ�� ���� �������� �Ա� ��û ���� ����ŭ �߰�
		ptr->user.money += atoi(ptr->msg);

		// �Ա� ��� ���� ����
		ptr->state = DEPOSIT_RESULT_STATE;
		break;
		
	// ��� ���� �� ����
	case AUCTION_DEPOSIT_RESULT_STATE:
		// ��� �Լ� ����
		AuctionProcess(ptr);
		break;
	}
}

// �� ���ŷ �۽� ��� �Լ�
void Send_PackingProcess(Client_Info* ptr)
{
	// ����� ���� ����
	RESULT result = NODATA;
	PROTOCOL protocol;

	int retval;

	// ���¿� ���� �з�
	switch (ptr->state)
	{
	// ȸ������, �α���, ������ �޴� �۽�
	case INTRO_STATE:
		ptr->sendbytes = Packing(ptr->send_buf, INTRO, INTRO_MSG);
		// �޴� ���� ���·� ����
		ptr->state = MENU_SELECT_STATE;
		break;

	// ID, PW �޼��� �۽�
	case INFO_STATE:
		// ȸ������, �α��ο� �°� �ι�° ���������� Packing
		switch (atoi(ptr->msg))
		{
		case NEW_MENU:
			ptr->sendbytes = Packing(ptr->send_buf, ID_PW_MSG, SECOND_NEW, ID_MSG, PW_MSG);
			break;

		case LOGIN_MENU:
			ptr->sendbytes = Packing(ptr->send_buf, ID_PW_MSG, SECOND_LOGIN, ID_MSG, PW_MSG);
			break;
		}
		// ���� ID, PW ���� ���� ���·� ����
		ptr->state = USER_INFO_STATE;
		break;

	// ȸ������ ����
	case NEW_STATE:
		// ȸ������ �Լ� ����
		JoinProcess(ptr);
		break;

	// �α��� ����
	case LOGIN_STATE:
		// �α��� �Լ� ����
		LoginProcess(ptr);
		break;

	// �α��� �޴� (��� ����, ��� ��ǰ, �Ա� ��û, �ܾ� Ȯ��, �α׾ƿ�, ȸ��Ż��)
	case LOGIN_INTRO_STATE:
		// �޴� �� Packing
		ptr->sendbytes = Packing(ptr->send_buf, LOGIN_INTRO, LOGIN_INTRO_MSG);
		// �޴� ���� ���·� ����
		ptr->state = MENU_SELECT_STATE;
		break;

	// ��� ����Ʈ
	case AUCTION_INTRO_STATE:
		// ��� ����Ʈ �Լ� ���� ( ��� ����Ʈ �۽� )
		AuctionIntroProcess(ptr);
		break;

	// ��� ������ MSG �۽� ����
	case AUCTION_DEPOSIT_STATE:
		ptr->sendbytes = Packing(ptr->send_buf, AUCTION_DEPOSIT_INFO, DEPOSIT_MSG);
		// ���� �ݾ� Ȯ�� ���·� ����
		ptr->state = AUCTION_DEPOSIT_RESULT_STATE;
		break;

	// ��� ���� ����
	case AUCTION_ERROR_STATE:
		// ��� ���� ��� ��� �� Packing
		ptr->sendbytes = Packing(ptr->send_buf, AUCTION_ERROR, AUCTION_ERROR_MSG, LOGIN_INTRO_MSG);
		// �޴� ���� ���·� ����
		ptr->state = MENU_SELECT_STATE;
		break;

	// ��� ���� ���� ��, ��� ���� ��ٸ��� ����
	case AUCTION_WAIT_MSG_STATE:
		// ��� ���� ��ٸ��ٴ� MSG Packing
		ptr->sendbytes = Packing(ptr->send_buf, AUCTION_WAIT, AUCTION_WAIT_MSG);

		// ������� ���� ������ ��ٸ��� ����
		// �Ʒ� ���´� recv, send �ƹ��͵� ���� �ʴ� �׿� ����
		// ��� ��� �Լ����� ��� ���� Ŭ���̾�Ʈ ����Ʈ ��ü�� ���鼭 ���¸� �������� ������
		// �׿� ���¿��� ���
		ptr->state = AUCTION_WAIT_STATE;
		break;

	// ��� ���� ���� ����
	case AUCTION_FAIL_STATE:
		// ��� ���� ���� ��� ���� �α��� �޴� ��� �� Packing
		ptr->sendbytes = Packing(ptr->send_buf, AUCTION_RESULT, AUCTION_FAIL_MSG, LOGIN_INTRO_MSG);
		// �޴� ���� ���·� ����
		ptr->state = MENU_SELECT_STATE;
		break;

	// ��� ���� ���� ����
	case AUCTION_SUCCESS_STATE:
		// ��� ���� ���� ��� ���� �α��� �޴� ��� �� Packing
		ptr->sendbytes = Packing(ptr->send_buf, AUCTION_RESULT, AUCTION_SUCCESS_MSG, LOGIN_INTRO_MSG);
		// �޴� ���� ���·� ����
		ptr->state = MENU_SELECT_STATE;
		break;

	// ��� ��ǰ Ȯ�� ����
	case WARE_STATE:
		// ���� ���� ��� ��ǰ Ȯ�� �Լ� ����
		WareViewProcess(ptr);
		break;

	// �Ա� ����
	case DEPOSIT_STATE:
		// �Ա� MSG Packing
		ptr->sendbytes = Packing(ptr->send_buf, DEPOSIT, SECOND_LOGIN, DEPOSIT_MSG);
		// �Ա� ���� ���·� ����
		ptr->state = DEPOSIT_INFO_STATE;
		break;

	// ���� �ܾ� Ȯ�� ����
	case MONEY_STATE:
		// ���� �ܾ� ��� ���� �α��� �޴� ��� �� Packing
		sprintf_s(ptr->msg, "\n���� �ܾ� : %d\n", ptr->user.money);
		ptr->sendbytes = Packing(ptr->send_buf, MONEY_VIEW, ptr->msg, LOGIN_INTRO_MSG);
		// �޴� ���� ���·� ����
		ptr->state = MENU_SELECT_STATE;
		break;

	// �Ա� ��� ����
	case DEPOSIT_RESULT_STATE:
		// �������� �и�
		protocol = GetProtocol(ptr->recv_buf);

		// �������ݿ� ���� �۾�
		switch (protocol)
		{
		// ȸ�� ���� ��, ȸ�� ���� ���� MSG ��� �� Packing
		// ���� Ŭ���̾�Ʈ ���� ������ ���� ����Ʈ�� �߰�
		case DEPOSIT_NEW:
			ptr->sendbytes = Packing(ptr->send_buf, NEW_RESULT, NEW_SUCCESS, NEW_MSG);
			userlist[User_Count++] = ptr->user;

			// ���� �޴� ���·� ����
			ptr->state = INTRO_STATE;
			break;

		// �α��� ��, �Ա� ��û �Ϸ� ���� �¶��� ������ �ܰ� ��� �� Packing
		case DEPOSIT_LOGIN:
			ZeroMemory(ptr->msg, sizeof(ptr->msg));
			sprintf_s(ptr->msg, "\n%s\n���� �¶��� ���� �ܰ� : %d\n", DISPOSIT_SUCCESS_MSG, ptr->user.money);
			ptr->sendbytes = Packing(ptr->send_buf, DEPOSIT_SUCCESS, ptr->msg);

			// �α��� �޴� ���·� ����
			ptr->state = LOGIN_INTRO_STATE;
			break;
		}
		break;

	// �α׾ƿ� ����
	case LOGOUT_STATE:
		// �α׾ƿ� �Լ� ����
		LogoutProcess(ptr);
		break;

	// ȸ��Ż�� ����
	case REMOVE_STATE:
		// ȸ��Ż�� �Լ� ����
		RemoveProcess(ptr);
		break;
	}
}

// Ŭ���̾�Ʈ ���� �߰�
bool Add_ClientInfo(SOCKET sock, SOCKADDR_IN addr)
{
	// ���� ������ Ŭ���̾�Ʈ �ο��� FD_SETSIZE �� �ʰ� �� ���
	if (Client_Count >= FD_SETSIZE)
	{
		printf("\n[Error] Ŭ���̾�Ʈ�� �� �̻� �߰� �� �� �����ϴ�. ( �ִ� 64�� )\n");
		return false;
	}

	// ���� �Ҵ�
	Client_Info* ptr = new Client_Info;

	// ���� �Ҵ翡 ������ ���
	if (ptr == nullptr)
	{
		printf("\n[Error] Ŭ���̾�Ʈ ���� �Ҵ翡 �����Ͽ����ϴ�.\n");
		return false;
	}

	// Ŭ���̾�Ʈ ���� ����ü ���� ������ �ʱ�ȭ �۾�
	ptr->sock = sock;
	memcpy(&ptr->addr, &addr, sizeof(SOCKADDR_IN));

	ptr->total_recvbytes = 0;
	ptr->total_sendbytes = 0;

	ptr->recvbytes = 0;
	ptr->sendbytes = 0;

	ZeroMemory(&ptr->user, sizeof(User_Info));
	ptr->state = INTRO_STATE;

	ptr->recv_flag = false;

	// �ʱ�ȭ �۾��� ���� �� Ŭ���̾�Ʈ ���� ����Ʈ�� �߰�
	Client_Array[Client_Count++] = ptr;

	printf("\n[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));
	return true;
}

// Ŭ���̾�Ʈ ���� ����
void Remove_ClientInfo(Client_Info* ptr, int& index)
{
	printf("\n[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));

	for (int i = 0; i < Client_Count; i++)
	{
		// ���� ������ ������ Ŭ���̾�Ʈ ã��
		if (Client_Array[i]->sock == ptr->sock)
		{
			// Ŭ���̾�Ʈ�� ������ ���� ������ ���� ����Ʈ�� ��������
			for (int j = 0; j < User_Count; j++)
			{
				if (!strcmp(userlist[j].id, ptr->user.id))
				{
					ptr->user.state = false;
					ptr->user.deposit_money = 0;

					userlist[j] = ptr->user;
					break;
				}
			}

			// ���� Ŭ���̾�Ʈ�� ��ſ� ���� �� ������ ������ ���
			// ��ſ��� Ŭ���̾�Ʈ ������ ����
			for (int j = 0; j < Auction_Count; j++)
			{
				for (int k = 0; k < Auction_List[j].count; k++)
				{
					if (Auction_List[j].client[k]->sock == ptr->sock)
					{
						for (int m = k; m < Auction_List[j].count - 1; m++)
						{
							Auction_List[j].client[m] = Auction_List[j].client[m + 1];
						}

						Auction_List[j].client[Auction_List[j].count - 1] = nullptr;
						Auction_List[j].count--;
						break;
					}
				}
			}

			// ���� ���� �� Ŭ���̾�Ʈ ����Ʈ���� ���� ����
			closesocket(ptr->sock);
			delete Client_Array[i];

			// Ŭ���̾�Ʈ ����Ʈ �迭�� �� ������ ����ִ� �۾�
			for (int j = i; j < Client_Count - 1; j++)
			{
				Client_Array[j] = Client_Array[j + 1];
			}

			Client_Array[Client_Count - 1] = nullptr;
			Client_Count--;
			index--;
			break;
		}
	}
}

// �޴�����
void MenuSelectProcess(Client_Info* ptr, PROTOCOL protocol)
{
	// ���� ���� ����
	ZeroMemory(ptr->msg, sizeof(ptr->msg));
	UnPacking(ptr->recv_buf, ptr->msg);

	// �������ݰ� ������ �޴� ���ÿ� ���� ���¸� ����
	switch (protocol)
	{
	case INTRO_SELECT_MENU:
		switch (atoi(ptr->msg))
		{
		case NEW_MENU:
		case LOGIN_MENU:
			ptr->state = INFO_STATE;
			break;

		case EXIT_MENU:
			ptr->state = DISCONNECTED_STATE;
			break;

		default:
			ptr->state = INTRO_STATE;
			break;
		}
		break;

	case LOGIN_SELECT_MENU:
		switch (atoi(ptr->msg))
		{
		case AUCTION_MENU:
			ptr->state = AUCTION_INTRO_STATE;
			break;

		case WARE_MENU:
			ptr->state = WARE_STATE;
			break;

		case DEPOSIT_MENU:
			ptr->state = DEPOSIT_STATE;
			break;

		case MONEY_MENU:
			ptr->state = MONEY_STATE;
			break;

		case LOGOUT_MENU:
			ptr->state = LOGOUT_STATE;
			break;

		case REMOVE_MENU:
			ptr->state = REMOVE_STATE;
			break;

		default:
			ptr->state = LOGIN_INTRO_STATE;
			break;
		}
		break;

	case AUCTION_SELECT:
		if (atoi(ptr->msg) == Auction_Count + 1)
		{
			ptr->state = LOGIN_INTRO_STATE;
		}
		else if (atoi(ptr->msg) > Auction_Count + 1)
		{
			ptr->state = AUCTION_INTRO_STATE;
		}
		else
		{
			ptr->state = AUCTION_DEPOSIT_STATE;
		}
		break;
	}
}

// ȸ������
void JoinProcess(Client_Info* ptr)
{
	for (int i = 0; i < User_Count; i++)
	{
		// ���� ����Ʈ�� �ߺ��Ǵ� ID ���� �˻�
		if (!strcmp(userlist[i].id, ptr->user.id))
		{
			// �ߺ��Ǵ� ID �� �ִٸ� ID �ߺ� ���� Packing
			ptr->sendbytes = Packing(ptr->send_buf, NEW_RESULT, ID_EXIST, NEW_ERROR_MSG);
			ptr->state = INTRO_STATE;
			return;
		}
	}

	// �ߺ����� �ʴ´ٸ� �ʱ� �Ա� MSG Packing
	ptr->sendbytes = Packing(ptr->send_buf, DEPOSIT, SECOND_NEW, DEPOSIT_MSG);
	ptr->state = DEPOSIT_INFO_STATE;
}

// �α���
void LoginProcess(Client_Info* ptr)
{
	for (int i = 0; i < User_Count; i++)
	{
		// ��ġ�ϴ� ID �� �ִ� �� �˻�
		if (!strcmp(userlist[i].id, ptr->user.id))
		{
			// PW �� ��ġ�ϴ� �� �˻�
			if (!strcmp(userlist[i].pw, ptr->user.pw))
			{
				// �ش� ������ ���� ���� ������ �� �ƴ� �� �˻�
				if (!userlist[i].state)
				{
					// ��� ���� ���� ��, �α��� ���� �� ��� ��� Packing
					ptr->sendbytes = Packing(ptr->send_buf, LOGIN_RESULT, LOGIN_SUCCESS, LOGIN_MSG);
					ptr->state = LOGIN_INTRO_STATE;

					// �ش� ������ ���� ���·� ���� �� ���� Ŭ���̾�Ʈ ������ �����ͷ� ����
					userlist[i].state = true;
					ptr->user = userlist[i];
					return;
				}

				// �ش� ������ ���� ���� �����̶��
				else
				{
					// ���� ��� ��� Packing
					ptr->sendbytes = Packing(ptr->send_buf, LOGIN_RESULT, LOGIN_ERROR, OVERRAP_ERROR_MSG);
					ptr->state = INTRO_STATE;
					return;
				}
			}
			// PW �� ��ġ���� ���� ���
			else
			{
				// ���� ��� ��� Packing
				ptr->sendbytes = Packing(ptr->send_buf, LOGIN_RESULT, PW_ERROR, PW_ERROR_MSG);
				ptr->state = INTRO_STATE;
				return;
			}
			break;
		}
	}
	
	// ��ġ�ϴ� ID �� ã�� ������ ��� ���� ��� ��� Packing
	ptr->sendbytes = Packing(ptr->send_buf, LOGIN_RESULT, ID_ERROR, ID_ERROR_MSG);
	ptr->state = INTRO_STATE;
}

// ��� ����Ʈ
void AuctionIntroProcess(Client_Info* ptr)
{
	// ��� ����Ʈ�� �ƹ��͵� ���� ��� ( ��� ��� ��ǰ�� ���� �Ǿ��� ��� )
	if (Auction_Count <= 0)
	{
		// ��Ű� �����ٴ� �޼��� Packing
		ptr->sendbytes = Packing(ptr->send_buf, AUCTION_END, NONE_AUCTION_MSG, LOGIN_INTRO_MSG);
		ptr->state = MENU_SELECT_STATE;
		return;
	}

	// ��� ����Ʈ�� ���� char �� ���� �غ�
	char list[MSGSIZE * 3];
	int len = 0;
	ZeroMemory(list, sizeof(list));

	// ��� ����Ʈ �޼����� �������� �̾� ���δ�.
	strcpy(list, AUCTION_INTRO_MSG);

	for (int i = 0; i < Auction_Count; i++)
	{
		sprintf(ptr->msg, "%d. %s / ���� �ݾ� : %d / ���� ���� �ο�(Max : %d) : %d\n"
			, i + 1, Auction_List[i].ware.name, Auction_List[i].ware.min_money,
			Auction_List[i].max_Count, Auction_List[i].count);

		strcat(list, ptr->msg);
	}

	sprintf(ptr->msg, "\n%d. ������\n\n�Է� : ", Auction_Count + 1);
	strcat(list, ptr->msg);

	// �ϼ��� ����Ʈ�� Packing
	ptr->sendbytes = Packing(ptr->send_buf, AUCTIONLIST_VIEW, list);
	ptr->state = MENU_SELECT_STATE;
}

// ���
void AuctionProcess(Client_Info* ptr)
{
	// Ŭ���̾�Ʈ�� ������ ��� ��ǰ index
	int index = atoi(ptr->msg);
	index--;	// �迭�� ���߱� ���� ����

	// ���� ���� ����
	ZeroMemory(ptr->msg, sizeof(ptr->msg));
	UnPacking(ptr->recv_buf, ptr->msg);
	
	// �Է� ���� ���� �ݾ� ����
	int money = atoi(ptr->msg);

	// ���� �ݾ��� ��� ��ǰ�� ���� ���ݺ��� ���ų�, �ڽ��� ���� ������ ���� �ݾ��� ���� ���� ���
	if (Auction_List[index].ware.min_money > money ||
		money > ptr->user.money)
	{
		// ��� ���� ���·� ����
		ptr->state = AUCTION_ERROR_STATE;
		return;
	}

	// ���� �ݾ��� ���� ������ ����
	ptr->user.deposit_money = money;
	// ��� �ý����� Ŭ���̾�Ʈ ����Ʈ�� ���� Ŭ���̾�Ʈ ������ �߰�
	Auction_List[index].client[Auction_List[index].count++] = ptr;

	// ���� Ŭ���̾�Ʈ�� �����ϸ鼭 ��� �ִ� ���� �ο��� ���� �Ǿ��� ��� 
	if (Auction_List[index].count >= Auction_List[index].max_Count)
	{		
		// ��� ��� �Լ� ����
		AuctionResultProcess(index);
		return;
	}

	// ��� �ִ� ���� �ο��� �������� ���� ���
	else
	{
		// ����� ��ٸ� ������ ����ϴ� ���·� ����
		ptr->state = AUCTION_WAIT_MSG_STATE;
		return;
	}
}

// ��� ���
void AuctionResultProcess(int index)
{
	// Ŭ���̾�Ʈ ���� �ӽ� ������ ��� ����Ʈ ���� ù��° Ŭ���̾�Ʈ ������ ����
	Client_Info* temp = Auction_List[index].client[0];

	// ��ſ� ������ Ŭ���̾�Ʈ �߿��� ���� ���� ���� �ݾ��� �����Ͽ��°� �� ���ϴ� �۾�
	// ���� ���� ���� �ݾ��� �θ� Ŭ���̾�Ʈ�� 2 ���̶�� ���� �ݾ��� �θ� Ŭ���̾�Ʈ���� ����
	for (int i = 0; i < Auction_List[index].max_Count - 1; i++)
	{
		if (temp->user.deposit_money <
			Auction_List[index].client[i + 1]->user.deposit_money)
		{
			temp = Auction_List[index].client[i + 1];
		}
	}

	// ���� ���� �ݾ��� ������ ������ ��� ��ǰ ����Ʈ�� ���� ����� ��ǰ ������ �߰�
	temp->user.warelist[temp->user.count++] = Ware_Info(Auction_List[index].ware.name, Auction_List[index].ware.min_money);

	// ������ ó�� �۾�
	for (int i = 0; i < Auction_List[index].max_Count; i++)
	{
		Client_Info* ptr2 = Auction_List[index].client[i];

		// ��� ������ ������ Ŭ���̾�Ʈ ����
		if (ptr2->sock == temp->sock)
		{
			// Ŭ���̾�Ʈ�� ���� �������� �����ߴ� �ݾ׸�ŭ ����
			ptr2->user.money -= ptr2->user.deposit_money;

			// ���� ����Ʈ�� Ŭ���̾�Ʈ�� ���� ������ ����
			for (int j = 0; j < User_Count; j++)
			{
				if (!strcmp(userlist[j].id, ptr2->user.id))
				{
					userlist[j] = ptr2->user;
					break;
				}
			}

			// ���� ���� ���·� ����
			ptr2->state = AUCTION_SUCCESS_STATE;
		}

		// �� �ܿ� ������ Ŭ���̾�Ʈ ������
		else
		{
			// ���� ���� ���·� ����
			ptr2->state = AUCTION_FAIL_STATE;
		}

		// ���� ���� �ʱ�ȭ
		ptr2->user.deposit_money = 0;
	}

	// ��� ����Ʈ���� ���� �� ��� ��ǰ�� ���� �۾�
	for (int i = index; i < Auction_Count - 1; i++)
	{
		Auction_Info temp = Auction_List[i];
		Auction_List[i] = Auction_List[i + 1];
		Auction_List[i + 1] = temp;
	}

	ZeroMemory(&Auction_List[Auction_Count - 1], sizeof(Auction_Info));
	Auction_Count--;
}

// ������ ���� ��� ��ǰ 
void WareViewProcess(Client_Info* ptr)
{
	// ��ǰ ����Ʈ�� ���� char �� ����
	char list[MSGSIZE * 5];
	ZeroMemory(list, sizeof(list));

	// ���� Ŭ���̾�Ʈ�� ���� ��� ��ǰ�� ���� ���
	if (ptr->user.count == 0)
	{
		ptr->sendbytes = Packing(ptr->send_buf, WARE_VIEW, NONE_WARE_MSG, LOGIN_INTRO_MSG);
		ptr->state = MENU_SELECT_STATE;
		return;
	}

	// ���� Ŭ���̾�Ʈ�� ���� ��� ��ǰ�� ����Ʈ �غ�
	for (int i = 0; i < ptr->user.count; i++)
	{
		sprintf_s(ptr->msg, "\n%d. %s\n", i + 1, ptr->user.warelist[i].name);
		strcat_s(list, ptr->msg);
	}

	// ��� ��ǰ ����Ʈ Packing
	ptr->sendbytes = Packing(ptr->send_buf, WARE_VIEW, list, LOGIN_INTRO_MSG);
	ptr->state = MENU_SELECT_STATE;
}

// �α׾ƿ�
void LogoutProcess(Client_Info* ptr)
{
	// ���� Ŭ���̾�Ʈ�� ���� ���� ������ ���� ã��
	for (int i = 0; i < User_Count; i++)
	{
		if (!strcmp(userlist[i].id, ptr->user.id))
		{
			// ���� ���¸� ������Ű��, ���� ����Ʈ�� Ŭ���̾�Ʈ�� ���� ������ ����
			ptr->user.state = false;
			userlist[i] = ptr->user;
			break;
		}
	}

	// �α׾ƿ� ��� �� Packing
	ptr->sendbytes = Packing(ptr->send_buf, LOGOUT, LOGOUT_MSG);
	ptr->state = INTRO_STATE;
}

// ȸ��Ż��
void RemoveProcess(Client_Info* ptr)
{
	// ���� Ŭ���̾�Ʈ�� ���� ���� ������ ���� ã��
	for (int i = 0; i < User_Count; i++)
	{
		if (!strcmp(userlist[i].id, ptr->user.id))
		{
			// ���� �� ������ ������ �迭�� ������ �̵� �� ������ �迭 ��ĭ �� ������ �̴� �۾�
			for (int j = i; j < User_Count - 1; j++)
			{
				userlist[j] = userlist[j + 1];
			}

			// ���� �� ���� ���� �ʱ�ȭ ���ѹ�����
			ZeroMemory(&userlist[User_Count - 1], sizeof(User_Info));
			User_Count--;
			break;
		}
	}

	// ��� ��� �� Packing
	ptr->sendbytes = Packing(ptr->send_buf, REMOVE, REMOVE_MSG);
	ptr->state = INTRO_STATE;
}