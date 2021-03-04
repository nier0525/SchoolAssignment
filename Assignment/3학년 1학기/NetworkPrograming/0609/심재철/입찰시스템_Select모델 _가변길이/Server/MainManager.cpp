#include "MainManager.h"
IMPLEMENT_SINGLETON(_MainManager)

_MainManager::_MainManager()
{
	Join_List = new CLinkedList<_User_Info*>();

	CM = _ClientManager::GetInstance();
	AM = _AuctionManager::GetInstance();
	AGM = _AuctionGruopManager::GetInstance();
	SM = _ServerManager::GetInstance();
}

_MainManager::~_MainManager()
{
	CM->Destroy();
	AM->Destroy();
	AGM->Destroy();
	SM->Destroy();

	DeleteCriticalSection(&cs);
	WSACleanup();
}

void _MainManager::Init()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		_ServerManager::GetInstance()->err_quit("WSAStartup()");
	InitializeCriticalSection(&cs);
	CreateThread(NULL, 0, CountThread, NULL, 0, NULL);

	SM->Init();

	AM->AddAuction("식탁", 3, 100000, cs);
	AM->AddAuction("침대", 2, 100000, cs);
	AM->AddAuction("장식장", 3, 100000, cs);
	AM->AddAuction("책상", 3, 100000, cs);
	AM->AddAuction("액자", 3, 100000, cs);

	printf("\nInit Complete\n");
}

void _MainManager::Run()
{
	while (1)
	{
		FD_ZERO(&Rset);
		FD_ZERO(&Wset);

		FD_SET(SM->Get_SocketInfo()->Get_ServerSock(), &Rset);
		FD_SET(SM->Get_SocketInfo()->Get_DummySock(), &Rset);

		CM->Get_UserList()->SearchStart();

		while (1)
		{
			EnterCriticalSection(&cs);

			_ClientInfo* ptr = CM->Get_UserList()->SearchData();
			if (ptr == nullptr)
			{
				LeaveCriticalSection(&cs);
				break;
			}

			FD_SET(ptr->Get_Socket(), &Rset);

			switch (ptr->Get_State())
			{
			case JOIN_RESULT_SEND_STATE:
			case LOGIN_RESULT_SEND_STATE:
			case USER_INFO_SEND_STATE:
			case AUCION_MONEY_UPDATE_SEND_STATE:
			case LOGOUT_RESULT_SEND_STATE:
			case USER_EXIT_INFO_SEND_STAE:
			case AUCTION_LIST_SEND_STATE:
			case AUCTION_BID_WAIT_SEND_STATE:
			case AUCTION_BID_START_SEND_STATE:
			case AUCTION_BID_SEND_STATE:
			case AUCTION_COUNT_SEND_STATE:
			case AUCTION_COMPLETE_STATE:
			case CONNECT_END_SEND_STATE:
			case AUCTION_ERROR_SEND_STATE:
				FD_SET(ptr->Get_Socket(), &Wset);
				break;
			}

			LeaveCriticalSection(&cs);
		}

		CM->Get_UserList()->SearchEnd();

		if (select(0, &Rset, &Wset, 0, NULL) == SOCKET_ERROR)
			SM->err_quit("select()");

		if (FD_ISSET(SM->Get_SocketInfo()->Get_ServerSock(), &Rset))
		{
			SOCKADDR_IN addr;
			ZeroMemory(&addr, sizeof(addr));
			int clntLen = sizeof(addr);
			SM->Get_SocketInfo()->Set_ClientSock(accept(SM->Get_SocketInfo()->Get_ServerSock(),
				(SOCKADDR*)&addr, &clntLen));
			SM->Get_SocketInfo()->Set_ClientAddr(addr);

			_ClientInfo* ptr = CM->AddClient(SM->Get_SocketInfo()->Get_ClientSock(),
				SM->Get_SocketInfo()->Get_ClientAddr(), cs);

			ptr->Set_State(LOGIN_MENU_SELECT_STATE);
			continue;
		}

		CM->Get_UserList()->SearchStart();

		while (1)
		{
			_ClientInfo* ptr = CM->Get_UserList()->SearchData();

			if (ptr == nullptr) break;

			if (FD_ISSET(ptr->Get_Socket(), &Rset))
			{
				int result = SM->PacketRecv(ptr);

				switch (result)
				{
				case DISCONNECTED:
					ptr->Set_State(DISCONNECTED_STATE);
					break;

				case SOC_FALSE:
					continue;

				case SOC_TRUE:
					break;
				}

				if (!Recv_Process(ptr)) continue;
			}

			if (FD_ISSET(ptr->Get_Socket(), &Wset))
			{
				if (!Send_Process(ptr)) continue;
			}
		}

		CM->Get_UserList()->SearchEnd();
	}
	return;
}

bool _MainManager::Recv_Process(_ClientInfo* ptr)
{
	PROTOCOL protocol;
	char msg[BUFSIZE];

	SM->GetProtocol(ptr->Get_RecvBuffer(), protocol);

	if (ptr->Get_State() != DISCONNECTED_STATE && protocol == CONNECT_END)
	{
		EnterCriticalSection(&cs);
		ptr->Set_PreState(ptr->Get_State());
		ptr->Set_State(CONNECT_END_SEND_STATE);
		LeaveCriticalSection(&cs);
		return false;
	}

	switch (ptr->Get_State())
	{
	case LOGIN_MENU_SELECT_STATE:
		EnterCriticalSection(&cs);

		char id[IDSIZE];
		ZeroMemory(id, sizeof(id));
		char pw[PWSIZE];
		ZeroMemory(pw, sizeof(pw));
		ZeroMemory(ptr->Get_Temp_UserInfo(), sizeof(_User_Info));

		switch (protocol)
		{
		case JOIN_INFO:
			char name[NICKNAMESIZE];
			ZeroMemory(name, sizeof(name));
			int money;

			SM->UnPackPacket(ptr->Get_RecvBuffer(), id, pw, name, money);
			ptr->Get_Temp_UserInfo()->Set_id(id);
			ptr->Get_Temp_UserInfo()->Set_pw(pw);
			ptr->Get_Temp_UserInfo()->Set_Nickname(name);
			ptr->Get_Temp_UserInfo()->Set_AuctionMoney(money);

			ptr->Set_State(JOIN_RESULT_SEND_STATE);
			break;

		case LOGIN_INFO:
			SM->UnPackPacket(ptr->Get_RecvBuffer(), id, pw);
			ptr->Get_Temp_UserInfo()->Set_id(id);
			ptr->Get_Temp_UserInfo()->Set_pw(pw);

			ptr->Set_State(LOGIN_RESULT_SEND_STATE);
			break;
		}

		LeaveCriticalSection(&cs);
		break;
		
	case AUCTION_MENU_SELECT_STATE:
		EnterCriticalSection(&cs);

		switch (protocol)
		{
		case REQ_AUCTION_LIST_INFO:
			ptr->Set_State(AUCTION_LIST_SEND_STATE);
			break;

		case AUCTION_SELECT_INFO:
			{
				int code = 0;

				SM->UnPackPacket(ptr->Get_RecvBuffer(), code);
				_AuctionInfo* auc_info = AM->SearchAuction(code);

				if (auc_info == nullptr)
				{
					ptr->Get_TryAuction()->Set_Result(AUCTION_CODE_ERROR);
					ptr->Set_PreState(ptr->Get_State());
					ptr->Set_State(AUCTION_ERROR_SEND_STATE);
					break;
				}

				_AuctionGroupInfo* group_info = AGM->AddAuctionGroup(auc_info, cs);

				if (group_info == nullptr)
				{
					ptr->Get_TryAuction()->Set_Result(AUCTION_GROUP_FULL_ERROR);
					ptr->Set_PreState(ptr->Get_State());
					ptr->Set_State(AUCTION_ERROR_SEND_STATE);
					break;
				}

				ptr->auction_start(auc_info);
				group_info->userlist_insert(ptr);
				ptr->Set_State(AUCTION_BID_WAIT_SEND_STATE);
			}
			break;

		case DEPOSIT_INFO:
			{
				int price;
				SM->UnPackPacket(ptr->Get_RecvBuffer(), price);
				ptr->deposit(price);

				ptr->Set_PreState(ptr->Get_State());
				ptr->Set_State(AUCION_MONEY_UPDATE_SEND_STATE);
			}
			break;

		case LOGOUT:
			ptr->logout();
			ptr->Set_State(LOGOUT_RESULT_SEND_STATE);
			break;
		}

		LeaveCriticalSection(&cs);
		break;

	case AUCTION_BID_STATE:
		EnterCriticalSection(&cs);
		{
			int price;
			_AuctionGroupInfo* group_info;

			switch (protocol)
			{
			case AUCTION_BID_PRICE:
				SM->UnPackPacket(ptr->Get_RecvBuffer(), price);

				if (!ptr->check_money(price))
				{
					ptr->Get_TryAuction()->Set_Result(AUCTION_SHORT_OF_MONEY_ERROR);
					ptr->Set_PreState(ptr->Get_State());
					ptr->Set_State(AUCTION_ERROR_SEND_STATE);
					break;
				}

				if (!AGM->TryBid(ptr, price))
				{
					ptr->Get_TryAuction()->Set_Result(AUCTION_BID_MONEY_ERROR);
					ptr->Set_PreState(ptr->Get_State());
					ptr->Set_State(AUCTION_ERROR_SEND_STATE);
					break;
				}

				group_info = AGM->SearchAuctionGroup(ptr->Get_TryAuction()->Get_AuctionInfo());
				group_info->searchstart();
				
				while (1)
				{
					_ClientInfo* client_info = nullptr;
					if (!group_info->searchdata(client_info)) break;

					client_info->Set_PreState(client_info->Get_State());
					client_info->Set_State(AUCTION_BID_SEND_STATE);
				}

				group_info->searchend();
				group_info->clear_timer_count();
				break;

			case LOGOUT:
				ptr->logout();
				ptr->Set_State(LOGOUT_RESULT_SEND_STATE);
				break;
			}
		}
		LeaveCriticalSection(&cs);
		break;

	case DISCONNECTED_STATE:
		EnterCriticalSection(&cs);
		AGM->ExitAuctionGroup(ptr);
		CM->RemoveClient(ptr, cs);
		LeaveCriticalSection(&cs);
		return false;
	}
	return true;
}

bool _MainManager::Send_Process(_ClientInfo* ptr)
{
	char msg[BUFSIZE];
	int size;

	int join_result = NODATA;
	int login_result = NODATA;
	int result;

	switch (ptr->Get_State())
	{
	case JOIN_RESULT_SEND_STATE:
		EnterCriticalSection(&cs);
		if (ptr->check_send_complete())
		{
			Join_List->SearchStart();

			while (1)
			{
				_User_Info* user = Join_List->SearchData();
				if (user == nullptr) break;

				if (!strcmp(user->Get_id(), ptr->Get_Temp_UserInfo()->Get_id()))
				{
					join_result = ID_EXIST;
					strcpy(msg, ID_EXIST_MSG);
					break;
				}
			}

			Join_List->SearchEnd();

			if (join_result == NODATA)
			{
				_User_Info* user = new _User_Info();
				user->Set_id(ptr->Get_Temp_UserInfo()->Get_id());
				user->Set_pw(ptr->Get_Temp_UserInfo()->Get_pw());
				user->Set_Nickname(ptr->Get_Temp_UserInfo()->Get_Nickname());
				user->Set_AuctionMoney(ptr->Get_Temp_UserInfo()->Get_AuctionMoney());

				Join_List->Insert(user);
				join_result = JOIN_SUCCESS;
				strcpy(msg, JOIN_SUCCESS_MSG);
			}

			SM->PackPacket(ptr->Set_SendBuffer(), JOIN_RESULT, join_result, msg, size);
			ptr->Set_SendByte(size);
		}

		if (SM->SendCheck(ptr) == SOC_FALSE || ptr->is_disconnected())
		{
			LeaveCriticalSection(&cs);
			return false;
		}

		ptr->Set_State(LOGIN_MENU_SELECT_STATE);
		LeaveCriticalSection(&cs);
		break;

	case LOGIN_RESULT_SEND_STATE:
		EnterCriticalSection(&cs);
		if (ptr->check_send_complete())
		{
			Join_List->SearchStart();

			while (1)
			{
				_User_Info* user_info = Join_List->SearchData();
				if (user_info == nullptr) break;

				if (!strcmp(user_info->Get_id(), ptr->Get_Temp_UserInfo()->Get_id()))
				{
					if (!strcmp(user_info->Get_pw(), ptr->Get_Temp_UserInfo()->Get_pw()))
					{
						ptr->login(user_info);
						login_result = LOGIN_SUCCESS;
						strcpy(msg, LOGIN_SUCCESS_MSG);
					}
					else
					{
						login_result = PW_ERROR;
						strcpy(msg, PW_ERROR_MSG);
					}
					break;
				}
			}

			Join_List->SearchEnd();
			if (login_result == NODATA)
			{
				login_result = ID_ERROR;
				strcpy(msg, ID_ERROR_MSG);
			}

			SM->PackPacket(ptr->Set_SendBuffer(), LOGIN_RESULT, login_result, msg, size);
			ptr->Set_SendByte(size);
		}

		if (SM->SendCheck(ptr) == SOC_FALSE || ptr->is_disconnected())
		{
			LeaveCriticalSection(&cs);
			return false;
		}

		if (login_result == LOGIN_SUCCESS)
		{
			ptr->Set_State(USER_INFO_SEND_STATE);
		}
		else
		{
			ptr->Set_State(LOGIN_MENU_SELECT_STATE);
		}
		LeaveCriticalSection(&cs);
		break;

	case USER_INFO_SEND_STATE:
		EnterCriticalSection(&cs);
		if (ptr->check_send_complete())
		{
			SM->PackPacket(ptr->Set_SendBuffer(), USER_INFO, ptr->Get_UserInfo()->Get_AuctionMoney(), ptr->Get_UserInfo()->Get_Nickname(), size);
			ptr->Set_SendByte(size);
		}

		if (SM->SendCheck(ptr) == SOC_FALSE || ptr->is_disconnected())
		{
			LeaveCriticalSection(&cs);
			return false;
		}

		ptr->Set_State(AUCTION_MENU_SELECT_STATE);
		LeaveCriticalSection(&cs);
		break;

	case AUCION_MONEY_UPDATE_SEND_STATE:
		EnterCriticalSection(&cs);
		if (ptr->check_send_complete())
		{
			SM->PackPacket(ptr->Set_SendBuffer(), AUCTION_MONEY_UPDATE_INFO, ptr->Get_UserInfo()->Get_AuctionMoney(), size);
			ptr->Set_SendByte(size);
		}

		if (SM->SendCheck(ptr) == SOC_FALSE || ptr->is_disconnected())
		{
			LeaveCriticalSection(&cs);
			return false;
		}

		ptr->Set_State(ptr->Get_PreState());
		LeaveCriticalSection(&cs);
		break;

	case AUCTION_LIST_SEND_STATE:
		EnterCriticalSection(&cs);
		if (ptr->check_send_complete())
		{
			if (AM->AllAuctionComplete())
			{
				SM->PackPacket(ptr->Set_SendBuffer(), ALL_AUCTION_COMPLETE, AUCTION_ALL_AUCTION_COMPLETE_MSG, size);
			}
			else
			{
				SM->PackPacket(ptr->Set_SendBuffer(), AUCTION_LIST_INFO, AM->Get_AuctionList(), size);
			}

			ptr->Set_SendByte(size);
		}

		if (SM->SendCheck(ptr) == SOC_FALSE || ptr->is_disconnected())
		{
			LeaveCriticalSection(&cs);
			return false;
		}

		ptr->Set_State(AUCTION_MENU_SELECT_STATE);
		LeaveCriticalSection(&cs);
		break;

	case AUCTION_BID_WAIT_SEND_STATE:
		EnterCriticalSection(&cs);
		if (ptr->check_send_complete())
		{
			SM->PackPacket(ptr->Set_SendBuffer(), AUCTION_BID_WAIT, AUCTION_BID_WAIT_MSG, size);
			ptr->Set_SendByte(size);
		}

		if (SM->SendCheck(ptr) == SOC_FALSE || ptr->is_disconnected())
		{
			LeaveCriticalSection(&cs);
			return false;
		}

		ptr->Set_State(AUCTION_BID_WAIT_STATE);
		LeaveCriticalSection(&cs);
	case AUCTION_BID_WAIT_STATE:
		EnterCriticalSection(&cs);
		{
			_AuctionGroupInfo* group_info = AGM->SearchAuctionGroup(ptr->Get_TryAuction()->Get_AuctionInfo());
			if (group_info->isfull())
			{
				group_info->searchstart();
				while (1)
				{
					_ClientInfo* client_info = nullptr;
					if (!group_info->searchdata(client_info)) break;

					client_info->Set_State(AUCTION_BID_START_SEND_STATE);
				}
				group_info->searchend();
			}
		}
		LeaveCriticalSection(&cs);
		break;

	case AUCTION_BID_START_SEND_STATE:
		EnterCriticalSection(&cs);
		if (ptr->check_send_complete())
		{
			SM->PackPacket(ptr->Set_SendBuffer(), AUCTION_BID_START, AUCTION_BID_START_MSG, size);
			ptr->Set_SendByte(size);
		}

		if (SM->SendCheck(ptr) == SOC_FALSE || ptr->is_disconnected())
		{
			LeaveCriticalSection(&cs);
			return false;
		}

		ptr->Set_State(AUCTION_BID_STATE);
		LeaveCriticalSection(&cs);
		break;

	case AUCTION_BID_SEND_STATE:
		EnterCriticalSection(&cs);
		if (ptr->check_send_complete())
		{
			_AuctionGroupInfo* group_info = AGM->SearchAuctionGroup(ptr->Get_TryAuction()->Get_AuctionInfo());
			ZeroMemory(msg, sizeof(msg));
			sprintf(msg, "%s 님이 %d 원을 입찰하셨습니다.\n", group_info->GetMaxBidUser()->Get_UserInfo()->Get_Nickname(),
				group_info->GetMaxBidUser()->Get_TryAuction()->Get_TryPrice());

			SM->PackPacket(ptr->Set_SendBuffer(), AUCTION_BID_PRICE, msg, size);
			ptr->Set_SendByte(size);
		}

		if (SM->SendCheck(ptr) == SOC_FALSE || ptr->is_disconnected())
		{
			LeaveCriticalSection(&cs);
			return false;
		}

		ptr->Set_State(ptr->Get_PreState());
		LeaveCriticalSection(&cs);
		break;

	case AUCTION_COUNT_SEND_STATE:
		EnterCriticalSection(&cs);
		if (ptr->check_send_complete())
		{
			_AuctionGroupInfo* group_info = AGM->SearchAuctionGroup(ptr->Get_TryAuction()->Get_AuctionInfo());
			sprintf(msg, "%d 초 지났습니다. 5초가 지나면 경매가 종료됩니다.\n", group_info->get_timer_count());
			SM->PackPacket(ptr->Set_SendBuffer(), AUCTION_SECOND_COUNT, msg, size);
			ptr->Set_SendByte(size);
		}

		if (SM->SendCheck(ptr) == SOC_FALSE || ptr->is_disconnected())
		{
			LeaveCriticalSection(&cs);
			return false;
		}

		{
			_AuctionGroupInfo* group_info = AGM->SearchAuctionGroup(ptr->Get_TryAuction()->Get_AuctionInfo());
			if (group_info->get_timer_count() >= MAX_AUCTION_COUNT)
			{
				if (group_info->GetMaxBidUser() == ptr)
				{
					ptr->try_auction_success();
					ptr->Get_TryAuction()->Get_AuctionInfo()->AuctionComplete(ptr->Get_UserInfo(), ptr->Get_TryAuction()->Get_TryPrice());
				}
				ptr->Set_State(AUCTION_COMPLETE_STATE);
				LeaveCriticalSection(&cs);
				break;
			}
		}

		ptr->Set_State(ptr->Get_PreState());
		LeaveCriticalSection(&cs);
		break;

	case AUCTION_COMPLETE_STATE:
		EnterCriticalSection(&cs);
		if (ptr->check_send_complete())
		{
			if (ptr->check_auction_success())
			{
				sprintf(msg, "%s 물품에 대한 경매에 성공했습니다. 낙찰가는 %d 원 입니다.\n",
					ptr->Get_TryAuction()->Get_AuctionInfo()->Get_Product(),
					ptr->Get_TryAuction()->Get_AuctionInfo()->Get_Price());
			}
			else
			{
				sprintf(msg, "%s 물품에 대한 경매에 실패했습니다. %s 님에게 낙찰되었으며 낙찰가는 %d 원 입니다.\n",
					ptr->Get_TryAuction()->Get_AuctionInfo()->Get_Product(),
					ptr->Get_TryAuction()->Get_AuctionInfo()->Get_MaxBidUser()->Get_Nickname(),
					ptr->Get_TryAuction()->Get_AuctionInfo()->Get_Price());
			}

			SM->PackPacket(ptr->Set_SendBuffer(), AUCTION_RESULT_INFO, msg, size);
			ptr->Set_SendByte(size);
		}

		if (SM->SendCheck(ptr) == SOC_FALSE || ptr->is_disconnected())
		{
			LeaveCriticalSection(&cs);
			return false;
		}

		if (AGM->CheckAuctionCompleteGroup(ptr)) 
		{

		}

		if (ptr->Get_TryAuction()->Get_Result() == AUCTION_SUCCESS)
		{
			ptr->auction_end();
			ptr->Set_PreState(AUCTION_MENU_SELECT_STATE);
			ptr->Set_State(AUCION_MONEY_UPDATE_SEND_STATE);
		}
		else
		{
			ptr->auction_end();
			ptr->Set_State(AUCTION_MENU_SELECT_STATE);
		}

		LeaveCriticalSection(&cs);
		break;

	case AUCTION_ERROR_SEND_STATE:
		EnterCriticalSection(&cs);
		if (ptr->check_send_complete())
		{
			ZeroMemory(msg, sizeof(msg));

			switch (ptr->Get_TryAuction()->Get_Result())
			{
			case AUCTION_GROUP_FULL_ERROR:
				strcpy(msg, AUCTION_FULL_ERROR_MSG);
				break;

			case AUCTION_CODE_ERROR:
				strcpy(msg, AUCTION_CODE_ERROR_MSG);
				break;

			case AUCTION_BID_MONEY_ERROR:
				strcpy(msg, AUCTION_BID_MONEY_ERROR_MSG);
				break;

			case AUCTION_SHORT_OF_MONEY_ERROR:
				strcpy(msg, AUCTION_MONEY_ERROR_MSG);
				break;
			}

			SM->PackPacket(ptr->Set_SendBuffer(), AUCTION_ERROR_INFO, msg, size);
			ptr->Set_SendByte(size);
		}

		if (SM->SendCheck(ptr) == SOC_FALSE || ptr->is_disconnected())
		{
			LeaveCriticalSection(&cs);
			return false;
		}

		ptr->Set_State(ptr->Get_PreState());
		LeaveCriticalSection(&cs);
		break;

	case CONNECT_END_SEND_STATE:
		EnterCriticalSection(&cs);
		if (ptr->check_send_complete())
		{
			SM->PackPacket(ptr->Set_SendBuffer(), CONNECT_END, size);
			ptr->Set_SendByte(size);
		}

		if (SM->SendCheck(ptr) == SOC_FALSE || ptr->is_disconnected())
		{
			LeaveCriticalSection(&cs);
			return false;
		}

		ptr->Set_State(ptr->Get_PreState());
		LeaveCriticalSection(&cs);
		break;

	case USER_EXIT_INFO_SEND_STAE:
		EnterCriticalSection(&cs);
		if (ptr->check_send_complete())
		{
			SM->PackPacket(ptr->Set_SendBuffer(), USER_EXIT_INFO, USER_EXIT_INFO_MSG, size);
			ptr->Set_SendByte(size);
		}

		if (SM->SendCheck(ptr) == SOC_FALSE || ptr->is_disconnected())
		{
			LeaveCriticalSection(&cs);
			return false;
		}

		ptr->Set_State(ptr->Get_PreState());
		LeaveCriticalSection(&cs);
		break;

	case LOGOUT_RESULT_SEND_STATE:
		EnterCriticalSection(&cs);
		if (ptr->check_send_complete())
		{
			SM->PackPacket(ptr->Set_SendBuffer(), LOGOUT_RESULT, LOGOUT_MSG, size);
			ptr->Set_SendByte(size);
		}

		if (SM->SendCheck(ptr) == SOC_FALSE || ptr->is_disconnected())
		{
			LeaveCriticalSection(&cs);
			return false;
		}

		AGM->ExitAuctionGroup(ptr);
		ptr->Set_State(LOGIN_MENU_SELECT_STATE);
		LeaveCriticalSection(&cs);
		break;
	}

	if (ptr->Get_State() == DISCONNECTED_STATE)
	{
		CM->RemoveClient(ptr, cs);
		return false;
	}
	return true;
}

DWORD CALLBACK CountThread(LPVOID ptr)
{
	char msg[BUFSIZE];
	ZeroMemory(msg, sizeof(msg));
	
	CRITICAL_SECTION cs = _MainManager::GetInstance()->Get_CriticalSection();

	while (1)
	{
		int result = WaitForSingleObject(_ServerManager::GetInstance()->Get_SocketInfo()->Get_Event(), 100);
		EnterCriticalSection(&cs);

		switch (result)
		{
		case WAIT_OBJECT_0:
			LeaveCriticalSection(&cs);
			continue;

		case WAIT_TIMEOUT:
			_AuctionGruopManager::GetInstance()->Get_AuctionGroupList()->SearchStart();

			while (1)
			{
				_AuctionGroupInfo* info = _AuctionGruopManager::GetInstance()->Get_AuctionGroupList()->SearchData();

				if (info == nullptr) break;
				if (info->GetMaxBidUser() == nullptr) continue;

				info->increase_timer_count();
				if (info->check_timer_count())
				{
					info->searchstart();

					while (1)
					{
						_ClientInfo* ptr = nullptr;
						if (!info->searchdata(ptr)) break;

						ptr->Set_PreState(ptr->Get_State());
						ptr->Set_State(AUCTION_COUNT_SEND_STATE);
					}
					
					info->searchend();
					sendto(_ServerManager::GetInstance()->Get_SocketInfo()->Get_DummySock(), msg, sizeof(int), 0,
						(SOCKADDR*)&_ServerManager::GetInstance()->Get_SocketInfo()->Get_DummyAddr(), 
						sizeof(_ServerManager::GetInstance()->Get_SocketInfo()->Get_DummyAddr()));
				}
			}

			_AuctionGruopManager::GetInstance()->Get_AuctionGroupList()->SearchEnd();
		}
		LeaveCriticalSection(&cs);
	}
}