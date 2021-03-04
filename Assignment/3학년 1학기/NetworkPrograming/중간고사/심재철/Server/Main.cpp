#include "MyFunction.h"

// 전역 변수
// 클라이언트 정보 리스트
Client_Info* Client_Array[FD_SETSIZE];
int Client_Count = 0;

// 유저 정보 리스트
User_Info userlist[USERSIZE];
int User_Count = 0;

// 경매 정보 리스트
Auction_Info Auction_List[5];
int Auction_Count = 0;


// 논 블로킹 수신 담당 함수
void Recv_PackingProcess(Client_Info* ptr);

// 논 블로킹 송신 담당 함수
void Send_PackingProcess(Client_Info* ptr);

// 클라이언트 정보 추가
bool Add_ClientInfo(SOCKET sock, SOCKADDR_IN addr);

// 클라이언트 정보 삭제
void Remove_ClientInfo(Client_Info* ptr, int& index);

// 메뉴 선택
void MenuSelectProcess(Client_Info* ptr, PROTOCOL protocol);

// 회원 가입
void JoinProcess(Client_Info* ptr);

// 로그인
void LoginProcess(Client_Info* ptr);

// 경매 리스트
void AuctionIntroProcess(Client_Info* ptr);

// 경매
void AuctionProcess(Client_Info* ptr);

// 경매 결과
void AuctionResultProcess(int index);

// 유저가 가진 경매 물품 출력
void WareViewProcess(Client_Info* ptr);

// 로그아웃
void LogoutProcess(Client_Info* ptr);

// 회원탈퇴
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

	// 데이터 통신에 사용될 변수
	FD_SET rset, wset;

	SOCKET sock;
	SOCKADDR_IN addr;
	int retval, addrlen, index;

	// 사전에 미리 경매 리스트 추가
	Auction_List[Auction_Count++] = Auction_Info("마이클 조던의 농구화", 50000, 3, 0);
	Auction_List[Auction_Count++] = Auction_Info("코비 브라이언트의 사인", 40000, 2, 0);
	Auction_List[Auction_Count++] = Auction_Info("제임스 하든의 유니폼", 30000, 2, 0);

	while (1)
	{
		// 읽기, 쓰기 셋 초기화
		FD_ZERO(&rset);
		FD_ZERO(&wset);
		// listen 의 읽기셋은 항상 켜둠
		FD_SET(listen_sock, &rset);

		// 클라이언트 rset, wset 갱신
		for (index = 0; index < Client_Count; index++)
		{
			// 클라이언트의 읽기 셋은 항상 켜둠
			FD_SET(Client_Array[index]->sock, &rset);

			// 상태에 따라 클라이언트의 쓰기 셋을 킴
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

		// 소켓 셋 검사(1) : 클라이언트 접속
		if (FD_ISSET(listen_sock, &rset))
		{
			// listen Queue 에 있는 클라이언트 정보 갱신
			addrlen = sizeof(addr);
			sock = accept(listen_sock, (SOCKADDR*)&addr, &addrlen);
			if (sock == INVALID_SOCKET)	err_display("acccept()");		
			// 클라이언트 정보 추가
			else	Add_ClientInfo(sock, addr);			
		}

		// 소켓 셋 검사(2) : 데이터 통신
		for (index = 0; index < Client_Count; index++)
		{
			Client_Info* ptr = Client_Array[index];

			// 현재 클라이언트의 읽기 셋이 켜져 있는 경우
			if (FD_ISSET(ptr->sock, &rset))
			{
				// 가변 길이 데이터 수신 함수
				int result = Packing_Recv(ptr);

				// result 반환 값에 따라 작업
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

				// 데이터를 모두 받았거나, 중간에 클라이언트가 나갔거나 한 경우 이 곳에 도달함
				// 수신한 데이터를 토대로 작업
				Recv_PackingProcess(ptr);
			}

			// 현재 클라이언트의 쓰기 셋이 켜져 있는 경우
			if (FD_ISSET(ptr->sock, &wset))
			{
				// 데이터를 모두 송신했다면 송신 후의 작업
				if (ptr->sendbytes == 0) Send_PackingProcess(ptr);

				// result 값은 현재 데이터를 모두 송신했는가? 와 중간에 오류가를 범했는가? 를 반환
				// 송신 상태를 체킹해주는 함수 ( 내부에서 송신해주면서 수시로 체킹해줌 )
				int result = Result_Send(ptr);

				// 송신 할 데이터가 남아있다면 continue
				if (result == SOCKET_FALSE) continue;
			}

			// 현재 클라이언트의 상태가 접속 끊겼을 경우
			if (ptr->state == DISCONNECTED_STATE)
			{
				// 클라이언트 정보 삭제 작업
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

// 논 블로킹 수신 담당 함수
void Recv_PackingProcess(Client_Info* ptr)
{
	// 프로토콜 분리
	PROTOCOL protocol = GetProtocol(ptr->recv_buf);

	// 상태에 따라 분류
	switch (ptr->state)
	{
	
	// 메뉴 입력 상태
	case MENU_SELECT_STATE:
		// 메뉴 선택 함수 실행
		MenuSelectProcess(ptr, protocol);
		break;

	// 유저의 ID,PW 정보 값 수신
	case USER_INFO_STATE:
		ZeroMemory(&ptr->user, sizeof(ptr->user));
		UnPacking(ptr->recv_buf, ptr->user.id, ptr->user.pw);

		// 회원가입, 로그인 프로토콜에 따라 상태 변경
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

	// 입금 신청 받은 값 수신
	case DEPOSIT_INFO_STATE:
		ZeroMemory(ptr->msg, sizeof(ptr->msg));
		UnPacking(ptr->recv_buf, ptr->msg);

		// 클라이언트의 유저 소지금을 입금 신청 받은 값만큼 추가
		ptr->user.money += atoi(ptr->msg);

		// 입금 결과 상태 변경
		ptr->state = DEPOSIT_RESULT_STATE;
		break;
		
	// 경매 입찰 값 수신
	case AUCTION_DEPOSIT_RESULT_STATE:
		// 경매 함수 실행
		AuctionProcess(ptr);
		break;
	}
}

// 논 블로킹 송신 담당 함수
void Send_PackingProcess(Client_Info* ptr)
{
	// 통신을 위한 변수
	RESULT result = NODATA;
	PROTOCOL protocol;

	int retval;

	// 상태에 따라 분류
	switch (ptr->state)
	{
	// 회원가입, 로그인, 나가기 메뉴 송신
	case INTRO_STATE:
		ptr->sendbytes = Packing(ptr->send_buf, INTRO, INTRO_MSG);
		// 메뉴 선택 상태로 변경
		ptr->state = MENU_SELECT_STATE;
		break;

	// ID, PW 메세지 송신
	case INFO_STATE:
		// 회원가입, 로그인에 맞게 두번째 프로토콜을 Packing
		switch (atoi(ptr->msg))
		{
		case NEW_MENU:
			ptr->sendbytes = Packing(ptr->send_buf, ID_PW_MSG, SECOND_NEW, ID_MSG, PW_MSG);
			break;

		case LOGIN_MENU:
			ptr->sendbytes = Packing(ptr->send_buf, ID_PW_MSG, SECOND_LOGIN, ID_MSG, PW_MSG);
			break;
		}
		// 유저 ID, PW 정보 수신 상태로 변경
		ptr->state = USER_INFO_STATE;
		break;

	// 회원가입 상태
	case NEW_STATE:
		// 회원가입 함수 실행
		JoinProcess(ptr);
		break;

	// 로그인 상태
	case LOGIN_STATE:
		// 로그인 함수 실행
		LoginProcess(ptr);
		break;

	// 로그인 메뉴 (경매 참가, 경매 물품, 입금 신청, 잔액 확인, 로그아웃, 회원탈퇴)
	case LOGIN_INTRO_STATE:
		// 메뉴 값 Packing
		ptr->sendbytes = Packing(ptr->send_buf, LOGIN_INTRO, LOGIN_INTRO_MSG);
		// 메뉴 선택 상태로 변경
		ptr->state = MENU_SELECT_STATE;
		break;

	// 경매 리스트
	case AUCTION_INTRO_STATE:
		// 경매 리스트 함수 실행 ( 경매 리스트 송신 )
		AuctionIntroProcess(ptr);
		break;

	// 경매 입찰액 MSG 송신 상태
	case AUCTION_DEPOSIT_STATE:
		ptr->sendbytes = Packing(ptr->send_buf, AUCTION_DEPOSIT_INFO, DEPOSIT_MSG);
		// 입찰 금액 확인 상태로 변경
		ptr->state = AUCTION_DEPOSIT_RESULT_STATE;
		break;

	// 경매 에러 상태
	case AUCTION_ERROR_STATE:
		// 경매 에러 결과 출력 값 Packing
		ptr->sendbytes = Packing(ptr->send_buf, AUCTION_ERROR, AUCTION_ERROR_MSG, LOGIN_INTRO_MSG);
		// 메뉴 선택 상태로 변경
		ptr->state = MENU_SELECT_STATE;
		break;

	// 경매 입찰 성공 후, 결과 값을 기다리는 상태
	case AUCTION_WAIT_MSG_STATE:
		// 결과 값을 기다린다는 MSG Packing
		ptr->sendbytes = Packing(ptr->send_buf, AUCTION_WAIT, AUCTION_WAIT_MSG);

		// 결과값이 나올 때까지 기다리는 상태
		// 아래 상태는 recv, send 아무것도 하지 않는 잉여 상태
		// 경매 결과 함수에서 경매 내에 클라이언트 리스트 전체를 돌면서 상태를 변경해줄 때까지
		// 잉여 상태에서 대기
		ptr->state = AUCTION_WAIT_STATE;
		break;

	// 경매 낙찰 실패 상태
	case AUCTION_FAIL_STATE:
		// 경매 낙찰 실패 출력 값과 로그인 메뉴 출력 값 Packing
		ptr->sendbytes = Packing(ptr->send_buf, AUCTION_RESULT, AUCTION_FAIL_MSG, LOGIN_INTRO_MSG);
		// 메뉴 선택 상태로 변경
		ptr->state = MENU_SELECT_STATE;
		break;

	// 경매 낙찰 성공 상태
	case AUCTION_SUCCESS_STATE:
		// 경매 낙찰 성공 출력 값과 로그인 메뉴 출력 값 Packing
		ptr->sendbytes = Packing(ptr->send_buf, AUCTION_RESULT, AUCTION_SUCCESS_MSG, LOGIN_INTRO_MSG);
		// 메뉴 선택 상태로 변경
		ptr->state = MENU_SELECT_STATE;
		break;

	// 경매 물품 확인 상태
	case WARE_STATE:
		// 소지 중인 경매 물품 확인 함수 실행
		WareViewProcess(ptr);
		break;

	// 입금 상태
	case DEPOSIT_STATE:
		// 입금 MSG Packing
		ptr->sendbytes = Packing(ptr->send_buf, DEPOSIT, SECOND_LOGIN, DEPOSIT_MSG);
		// 입금 정보 상태로 변경
		ptr->state = DEPOSIT_INFO_STATE;
		break;

	// 현재 잔액 확인 상태
	case MONEY_STATE:
		// 현재 잔액 출력 값과 로그인 메뉴 출력 값 Packing
		sprintf_s(ptr->msg, "\n현재 잔액 : %d\n", ptr->user.money);
		ptr->sendbytes = Packing(ptr->send_buf, MONEY_VIEW, ptr->msg, LOGIN_INTRO_MSG);
		// 메뉴 선택 상태로 변경
		ptr->state = MENU_SELECT_STATE;
		break;

	// 입금 결과 상태
	case DEPOSIT_RESULT_STATE:
		// 프로토콜 분리
		protocol = GetProtocol(ptr->recv_buf);

		// 프로토콜에 따라 작업
		switch (protocol)
		{
		// 회원 가입 시, 회원 가입 성공 MSG 출력 값 Packing
		// 이후 클라이언트 유저 정보를 유저 리스트에 추가
		case DEPOSIT_NEW:
			ptr->sendbytes = Packing(ptr->send_buf, NEW_RESULT, NEW_SUCCESS, NEW_MSG);
			userlist[User_Count++] = ptr->user;

			// 시작 메뉴 상태로 변경
			ptr->state = INTRO_STATE;
			break;

		// 로그인 시, 입금 신청 완료 이후 온라인 통장의 잔고 출력 값 Packing
		case DEPOSIT_LOGIN:
			ZeroMemory(ptr->msg, sizeof(ptr->msg));
			sprintf_s(ptr->msg, "\n%s\n현재 온라인 통장 잔고 : %d\n", DISPOSIT_SUCCESS_MSG, ptr->user.money);
			ptr->sendbytes = Packing(ptr->send_buf, DEPOSIT_SUCCESS, ptr->msg);

			// 로그인 메뉴 상태로 변경
			ptr->state = LOGIN_INTRO_STATE;
			break;
		}
		break;

	// 로그아웃 상태
	case LOGOUT_STATE:
		// 로그아웃 함수 실행
		LogoutProcess(ptr);
		break;

	// 회원탈퇴 상태
	case REMOVE_STATE:
		// 회원탈퇴 함수 실행
		RemoveProcess(ptr);
		break;
	}
}

// 클라이언트 정보 추가
bool Add_ClientInfo(SOCKET sock, SOCKADDR_IN addr)
{
	// 현재 접속한 클라이언트 인원이 FD_SETSIZE 를 초과 한 경우
	if (Client_Count >= FD_SETSIZE)
	{
		printf("\n[Error] 클라이언트를 더 이상 추가 할 수 없습니다. ( 최대 64명 )\n");
		return false;
	}

	// 동적 할당
	Client_Info* ptr = new Client_Info;

	// 동적 할당에 실패한 경우
	if (ptr == nullptr)
	{
		printf("\n[Error] 클라이언트 정보 할당에 실패하였습니다.\n");
		return false;
	}

	// 클라이언트 정보 구조체 내에 변수들 초기화 작업
	ptr->sock = sock;
	memcpy(&ptr->addr, &addr, sizeof(SOCKADDR_IN));

	ptr->total_recvbytes = 0;
	ptr->total_sendbytes = 0;

	ptr->recvbytes = 0;
	ptr->sendbytes = 0;

	ZeroMemory(&ptr->user, sizeof(User_Info));
	ptr->state = INTRO_STATE;

	ptr->recv_flag = false;

	// 초기화 작업을 끝낸 후 클라이언트 정보 리스트에 추가
	Client_Array[Client_Count++] = ptr;

	printf("\n[TCP 서버] 클라이언트 접속 : IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));
	return true;
}

// 클라이언트 정보 삭제
void Remove_ClientInfo(Client_Info* ptr, int& index)
{
	printf("\n[TCP 서버] 클라이언트 종료 : IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));

	for (int i = 0; i < Client_Count; i++)
	{
		// 현재 접속을 종료한 클라이언트 찾기
		if (Client_Array[i]->sock == ptr->sock)
		{
			// 클라이언트가 가졌던 유저 정보를 유저 리스트에 갱신해줌
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

			// 만약 클라이언트가 경매에 참여 중 강제로 나갔을 경우
			// 경매에서 클라이언트 정보를 삭제
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

			// 소켓 종료 및 클라이언트 리스트에서 정보 삭제
			closesocket(ptr->sock);
			delete Client_Array[i];

			// 클라이언트 리스트 배열의 빈 공간을 당겨주는 작업
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

// 메뉴선택
void MenuSelectProcess(Client_Info* ptr, PROTOCOL protocol)
{
	// 수신 버퍼 분할
	ZeroMemory(ptr->msg, sizeof(ptr->msg));
	UnPacking(ptr->recv_buf, ptr->msg);

	// 프로토콜과 각각의 메뉴 선택에 따라 상태를 변경
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

// 회원가입
void JoinProcess(Client_Info* ptr)
{
	for (int i = 0; i < User_Count; i++)
	{
		// 유저 리스트에 중복되는 ID 유무 검사
		if (!strcmp(userlist[i].id, ptr->user.id))
		{
			// 중복되는 ID 가 있다면 ID 중복 에러 Packing
			ptr->sendbytes = Packing(ptr->send_buf, NEW_RESULT, ID_EXIST, NEW_ERROR_MSG);
			ptr->state = INTRO_STATE;
			return;
		}
	}

	// 중복되지 않는다면 초기 입금 MSG Packing
	ptr->sendbytes = Packing(ptr->send_buf, DEPOSIT, SECOND_NEW, DEPOSIT_MSG);
	ptr->state = DEPOSIT_INFO_STATE;
}

// 로그인
void LoginProcess(Client_Info* ptr)
{
	for (int i = 0; i < User_Count; i++)
	{
		// 일치하는 ID 가 있는 지 검사
		if (!strcmp(userlist[i].id, ptr->user.id))
		{
			// PW 가 일치하는 지 검사
			if (!strcmp(userlist[i].pw, ptr->user.pw))
			{
				// 해당 계정이 접속 중인 상태인 지 아닌 지 검사
				if (!userlist[i].state)
				{
					// 모든 조건 만족 시, 로그인 성공 및 결과 출력 Packing
					ptr->sendbytes = Packing(ptr->send_buf, LOGIN_RESULT, LOGIN_SUCCESS, LOGIN_MSG);
					ptr->state = LOGIN_INTRO_STATE;

					// 해당 계정을 접속 상태로 변경 및 현재 클라이언트 유저의 데이터로 복사
					userlist[i].state = true;
					ptr->user = userlist[i];
					return;
				}

				// 해당 계정이 접속 중인 계정이라면
				else
				{
					// 에러 결과 출력 Packing
					ptr->sendbytes = Packing(ptr->send_buf, LOGIN_RESULT, LOGIN_ERROR, OVERRAP_ERROR_MSG);
					ptr->state = INTRO_STATE;
					return;
				}
			}
			// PW 가 일치하지 않을 경우
			else
			{
				// 에러 결과 출력 Packing
				ptr->sendbytes = Packing(ptr->send_buf, LOGIN_RESULT, PW_ERROR, PW_ERROR_MSG);
				ptr->state = INTRO_STATE;
				return;
			}
			break;
		}
	}
	
	// 일치하는 ID 를 찾지 못했을 경우 에러 결과 출력 Packing
	ptr->sendbytes = Packing(ptr->send_buf, LOGIN_RESULT, ID_ERROR, ID_ERROR_MSG);
	ptr->state = INTRO_STATE;
}

// 경매 리스트
void AuctionIntroProcess(Client_Info* ptr)
{
	// 경매 리스트에 아무것도 없는 경우 ( 모든 경매 물품이 낙찰 되었을 경우 )
	if (Auction_Count <= 0)
	{
		// 경매가 끝났다는 메세지 Packing
		ptr->sendbytes = Packing(ptr->send_buf, AUCTION_END, NONE_AUCTION_MSG, LOGIN_INTRO_MSG);
		ptr->state = MENU_SELECT_STATE;
		return;
	}

	// 경매 리스트를 담을 char 형 변수 준비
	char list[MSGSIZE * 3];
	int len = 0;
	ZeroMemory(list, sizeof(list));

	// 경매 리스트 메세지를 차례차례 이어 붙인다.
	strcpy(list, AUCTION_INTRO_MSG);

	for (int i = 0; i < Auction_Count; i++)
	{
		sprintf(ptr->msg, "%d. %s / 최저 금액 : %d / 현재 참여 인원(Max : %d) : %d\n"
			, i + 1, Auction_List[i].ware.name, Auction_List[i].ware.min_money,
			Auction_List[i].max_Count, Auction_List[i].count);

		strcat(list, ptr->msg);
	}

	sprintf(ptr->msg, "\n%d. 나가기\n\n입력 : ", Auction_Count + 1);
	strcat(list, ptr->msg);

	// 완성된 리스트를 Packing
	ptr->sendbytes = Packing(ptr->send_buf, AUCTIONLIST_VIEW, list);
	ptr->state = MENU_SELECT_STATE;
}

// 경매
void AuctionProcess(Client_Info* ptr)
{
	// 클라이언트가 선택한 경매 물품 index
	int index = atoi(ptr->msg);
	index--;	// 배열에 맞추기 위해 감소

	// 수신 버퍼 분할
	ZeroMemory(ptr->msg, sizeof(ptr->msg));
	UnPacking(ptr->recv_buf, ptr->msg);
	
	// 입력 받은 입찰 금액 저장
	int money = atoi(ptr->msg);

	// 입찰 금액이 경매 물품의 최저 가격보다 낮거나, 자신이 가진 돈보다 많은 금액을 입찰 했을 경우
	if (Auction_List[index].ware.min_money > money ||
		money > ptr->user.money)
	{
		// 경매 에러 상태로 변경
		ptr->state = AUCTION_ERROR_STATE;
		return;
	}

	// 입찰 금액을 유저 정보에 저장
	ptr->user.deposit_money = money;
	// 경매 시스템의 클라이언트 리스트에 현재 클라이언트 정보를 추가
	Auction_List[index].client[Auction_List[index].count++] = ptr;

	// 현재 클라이언트가 참여하면서 경매 최대 참여 인원이 충족 되었을 경우 
	if (Auction_List[index].count >= Auction_List[index].max_Count)
	{		
		// 경매 결과 함수 실행
		AuctionResultProcess(index);
		return;
	}

	// 경매 최대 참여 인원을 충족하지 않을 경우
	else
	{
		// 결과값 기다릴 때까지 대기하는 상태로 변경
		ptr->state = AUCTION_WAIT_MSG_STATE;
		return;
	}
}

// 경매 결과
void AuctionResultProcess(int index)
{
	// 클라이언트 정보 임시 변수에 경매 리스트 내에 첫번째 클라이언트 정보를 저장
	Client_Info* temp = Auction_List[index].client[0];

	// 경매에 참가한 클라이언트 중에서 누가 가장 많은 금액을 입찰하였는가 를 구하는 작업
	// 만일 가장 높은 금액을 부른 클라이언트가 2 명이라면 먼저 금액을 부른 클라이언트에게 낙찰
	for (int i = 0; i < Auction_List[index].max_Count - 1; i++)
	{
		if (temp->user.deposit_money <
			Auction_List[index].client[i + 1]->user.deposit_money)
		{
			temp = Auction_List[index].client[i + 1];
		}
	}

	// 가장 많은 금액을 입찰한 유저의 경매 물품 리스트에 현재 경매의 물품 정보를 추가
	temp->user.warelist[temp->user.count++] = Ware_Info(Auction_List[index].ware.name, Auction_List[index].ware.min_money);

	// 이후의 처리 작업
	for (int i = 0; i < Auction_List[index].max_Count; i++)
	{
		Client_Info* ptr2 = Auction_List[index].client[i];

		// 경매 낙찰에 성공한 클라이언트 유저
		if (ptr2->sock == temp->sock)
		{
			// 클라이언트의 현재 소지금을 입찰했던 금액만큼 감소
			ptr2->user.money -= ptr2->user.deposit_money;

			// 유저 리스트에 클라이언트의 유저 정보를 갱신
			for (int j = 0; j < User_Count; j++)
			{
				if (!strcmp(userlist[j].id, ptr2->user.id))
				{
					userlist[j] = ptr2->user;
					break;
				}
			}

			// 낙찰 성공 상태로 변경
			ptr2->state = AUCTION_SUCCESS_STATE;
		}

		// 그 외에 나머지 클라이언트 유저들
		else
		{
			// 낙찰 실패 상태로 변경
			ptr2->state = AUCTION_FAIL_STATE;
		}

		// 입찰 정보 초기화
		ptr2->user.deposit_money = 0;
	}

	// 경매 리스트에서 낙찰 된 경매 물품을 빼는 작업
	for (int i = index; i < Auction_Count - 1; i++)
	{
		Auction_Info temp = Auction_List[i];
		Auction_List[i] = Auction_List[i + 1];
		Auction_List[i + 1] = temp;
	}

	ZeroMemory(&Auction_List[Auction_Count - 1], sizeof(Auction_Info));
	Auction_Count--;
}

// 유저가 가진 경매 물품 
void WareViewProcess(Client_Info* ptr)
{
	// 물품 리스트를 담을 char 형 변수
	char list[MSGSIZE * 5];
	ZeroMemory(list, sizeof(list));

	// 현재 클라이언트가 가진 경매 물품이 없는 경우
	if (ptr->user.count == 0)
	{
		ptr->sendbytes = Packing(ptr->send_buf, WARE_VIEW, NONE_WARE_MSG, LOGIN_INTRO_MSG);
		ptr->state = MENU_SELECT_STATE;
		return;
	}

	// 현재 클라이언트가 가진 경매 물품의 리스트 준비
	for (int i = 0; i < ptr->user.count; i++)
	{
		sprintf_s(ptr->msg, "\n%d. %s\n", i + 1, ptr->user.warelist[i].name);
		strcat_s(list, ptr->msg);
	}

	// 경매 물품 리스트 Packing
	ptr->sendbytes = Packing(ptr->send_buf, WARE_VIEW, list, LOGIN_INTRO_MSG);
	ptr->state = MENU_SELECT_STATE;
}

// 로그아웃
void LogoutProcess(Client_Info* ptr)
{
	// 현재 클라이언트가 접속 중인 유저의 계정 찾기
	for (int i = 0; i < User_Count; i++)
	{
		if (!strcmp(userlist[i].id, ptr->user.id))
		{
			// 접속 상태를 해지시키고, 유저 리스트에 클라이언트의 유저 정보를 갱신
			ptr->user.state = false;
			userlist[i] = ptr->user;
			break;
		}
	}

	// 로그아웃 출력 값 Packing
	ptr->sendbytes = Packing(ptr->send_buf, LOGOUT, LOGOUT_MSG);
	ptr->state = INTRO_STATE;
}

// 회원탈퇴
void RemoveProcess(Client_Info* ptr)
{
	// 현재 클라이언트가 접속 중인 유저의 계정 찾기
	for (int i = 0; i < User_Count; i++)
	{
		if (!strcmp(userlist[i].id, ptr->user.id))
		{
			// 삭제 할 유저의 정보를 배열을 끝으로 이동 및 나머지 배열 한칸 씩 앞으로 미는 작업
			for (int j = i; j < User_Count - 1; j++)
			{
				userlist[j] = userlist[j + 1];
			}

			// 삭제 할 유저 정보 초기화 시켜버리기
			ZeroMemory(&userlist[User_Count - 1], sizeof(User_Info));
			User_Count--;
			break;
		}
	}

	// 결과 출력 값 Packing
	ptr->sendbytes = Packing(ptr->send_buf, REMOVE, REMOVE_MSG);
	ptr->state = INTRO_STATE;
}