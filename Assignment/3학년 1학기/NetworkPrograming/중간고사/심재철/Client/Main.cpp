#include "MyFunction.h"

int main()
{
	// WSA Init
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	// Socket and Addr Init
	SOCKET sock;
	SOCKADDR_IN addr;

	// 소켓 정보 초기화
	Socket_Init(sock, addr);

	// Connect
	int retval = connect(sock, (SOCKADDR*)&addr, sizeof(addr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	// 통신에 필요한 변수
	char buf[BUFSIZE + 1];
	char menu[BUFSIZE / 2];
	char msg[MSGSIZE];

	int size = 0;

	User_Info user = User_Info();

	PROTOCOL protocol;
	SECOND_PROTOCOL second_protocol;
	RESULT result;

	while (1)
	{
		// 수신한 버퍼 데이터 분리 함수
		if (!UnPackingRecv(sock, buf))
		{
			break;
		}

		// 프로토콜 분리
		protocol = GetProtocol(buf);

		// 프로토콜에 따라 작업
		switch (protocol)
		{
		// 메뉴 또는 리스트를 수신 후, 값을 선택해서 다시 송신해주는 작업
		case INTRO:
		case LOGIN_INTRO:
		case AUCTIONLIST_VIEW:
			ZeroMemory(menu, sizeof(menu));
			UnPacking(buf, menu);

			printf("%s", menu);
			scanf("%s", menu);

			switch (protocol)
			{
			case INTRO:
				size = Packing(buf, INTRO_SELECT_MENU, menu);
				break;

			case LOGIN_INTRO:
				size = Packing(buf, LOGIN_SELECT_MENU, menu);
				break;

			case AUCTIONLIST_VIEW:
				size = Packing(buf, AUCTION_SELECT, menu);
				break;
			}

			retval = send(sock, buf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("send");
			}
			break;

		// ID, PW 정보 입력 후 송신
		case ID_PW_MSG:
			ZeroMemory(&user, sizeof(User_Info));
			UnPacking(buf, second_protocol, user.id, user.pw);

			printf("%s", user.id);
			scanf("%s", user.id);
			printf("%s", user.pw);
			scanf("%s", user.pw);

			// 회원가입인 지, 로그인인 지 에 따라 다른 프로토콜을 보냄
			// 이후에 두 프로토콜로 나뉘어 전혀 다른 기능을 함
			switch (second_protocol)
			{
			case SECOND_NEW:
				size = Packing(buf, PROTOCOL::NEW_INFO, user.id, user.pw);
				break;

			case SECOND_LOGIN:
				size = Packing(buf, PROTOCOL::LOGIN_INFO, user.id, user.pw);
				break;
			}

			retval = send(sock, buf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("send");
			}
			break;

		// 입금 할 값 입력 후 송신
		case DEPOSIT:
			ZeroMemory(msg, sizeof(msg));
			UnPacking(buf, second_protocol, msg);

			printf("%s", msg);
			scanf("%s", msg);

			// 회원 가입 할 때인가, 로그인 후 입금 기능을 사용할 때인가 구분
			switch (second_protocol)
			{
			case SECOND_NEW:
				size = Packing(buf, DEPOSIT_NEW, msg);
				break;

			case SECOND_LOGIN:
				size = Packing(buf, DEPOSIT_LOGIN, msg);
				break;
			}

			retval = send(sock, buf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("send");
			}
			break;

		// 경매 입찰 가격 입력 후 송신
		case AUCTION_DEPOSIT_INFO:
			ZeroMemory(msg, sizeof(msg));
			UnPacking(buf, msg);

			printf("%s", msg);
			scanf("%s", msg);

			size = Packing(buf, AUCTION_DEPOSIT_RESULT, msg);
			retval = send(sock, buf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("send");
			}
			break;

		// 회원가입, 로그인 결과 수신 후 출력
		case NEW_RESULT:
		case LOGIN_RESULT:
			ZeroMemory(msg, sizeof(msg));
			UnPacking(buf, result, msg);

			printf("%s", msg);
			break;

		// 기타 결과 값 수신 후 출력
		case AUCTION_WAIT:
		case DEPOSIT_SUCCESS:
		case LOGOUT:
		case REMOVE:
			ZeroMemory(msg, sizeof(msg));
			UnPacking(buf, msg);

			printf("%s", msg);
			break;

		// 결과 값 수신 후 바로 로그인 메뉴 선택해서 송신
		case AUCTION_END:
		case AUCTION_ERROR:
		case AUCTION_RESULT:
		case WARE_VIEW:
		case MONEY_VIEW:
			ZeroMemory(msg, sizeof(msg));
			ZeroMemory(menu, sizeof(menu));

			UnPacking(buf, msg, menu);

			printf("%s", msg);
			printf("%s", menu);
			scanf("%s", menu);
			
			size = Packing(buf, LOGIN_SELECT_MENU, menu);

			retval = send(sock, buf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("send");
			}
			break;
		}
	}

	// closesocket()
	closesocket(sock);

	// WSACleanup()
	WSACleanup();

	return 0;
}