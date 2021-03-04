#include "MyFunction.h"

int main()
{
	// WSA Init
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	// Socket and Addr Init
	SOCKET sock;
	SOCKADDR_IN addr;

	// ���� ���� �ʱ�ȭ
	Socket_Init(sock, addr);

	// Connect
	int retval = connect(sock, (SOCKADDR*)&addr, sizeof(addr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	// ��ſ� �ʿ��� ����
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
		// ������ ���� ������ �и� �Լ�
		if (!UnPackingRecv(sock, buf))
		{
			break;
		}

		// �������� �и�
		protocol = GetProtocol(buf);

		// �������ݿ� ���� �۾�
		switch (protocol)
		{
		// �޴� �Ǵ� ����Ʈ�� ���� ��, ���� �����ؼ� �ٽ� �۽����ִ� �۾�
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

		// ID, PW ���� �Է� �� �۽�
		case ID_PW_MSG:
			ZeroMemory(&user, sizeof(User_Info));
			UnPacking(buf, second_protocol, user.id, user.pw);

			printf("%s", user.id);
			scanf("%s", user.id);
			printf("%s", user.pw);
			scanf("%s", user.pw);

			// ȸ�������� ��, �α����� �� �� ���� �ٸ� ���������� ����
			// ���Ŀ� �� �������ݷ� ������ ���� �ٸ� ����� ��
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

		// �Ա� �� �� �Է� �� �۽�
		case DEPOSIT:
			ZeroMemory(msg, sizeof(msg));
			UnPacking(buf, second_protocol, msg);

			printf("%s", msg);
			scanf("%s", msg);

			// ȸ�� ���� �� ���ΰ�, �α��� �� �Ա� ����� ����� ���ΰ� ����
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

		// ��� ���� ���� �Է� �� �۽�
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

		// ȸ������, �α��� ��� ���� �� ���
		case NEW_RESULT:
		case LOGIN_RESULT:
			ZeroMemory(msg, sizeof(msg));
			UnPacking(buf, result, msg);

			printf("%s", msg);
			break;

		// ��Ÿ ��� �� ���� �� ���
		case AUCTION_WAIT:
		case DEPOSIT_SUCCESS:
		case LOGOUT:
		case REMOVE:
			ZeroMemory(msg, sizeof(msg));
			UnPacking(buf, msg);

			printf("%s", msg);
			break;

		// ��� �� ���� �� �ٷ� �α��� �޴� �����ؼ� �۽�
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