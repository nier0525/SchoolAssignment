#include "MyFunction.h"

int main()
{
	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(SERVERIP);
	addr.sin_port = htons(SERVERPORT);

	retval = connect(sock, (SOCKADDR*)&addr, sizeof(addr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	char buf[BUFSIZE];
	int len;

	char msg[MSGSIZE];
	int size;
	bool flag = false;

	UserInfo user;
	ZeroMemory(&user, sizeof(user));

	PROTOCOL protocol;
	RESULT result;
	STATE state = MENU_SELECT_STATE;

	while (!flag)
	{
		switch (state)
		{
			case MENU_SELECT_STATE:
				printf("\n1. 회원가입\n2. 로그인\n3. 종료\n\n입력 : ");
				scanf("%s", msg);

				switch (atoi(msg))
				{
				case 1:
					ZeroMemory(&user, sizeof(user));

					printf("ID : ");
					scanf("%s", user.id);
					printf("PW : ");
					scanf("%s", user.pw);

					size = Packing(buf, JOIN_INFO, user.id, user.pw);
					retval = send(sock, buf, size, 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send");
					}

					state = RESULT_STATE;
					break;

				case 2:
					ZeroMemory(&user, sizeof(user));

					printf("ID : ");
					scanf("%s", user.id);
					printf("PW : ");
					scanf("%s", user.pw);

					size = Packing(buf, LOGIN_INFO, user.id, user.pw);
					retval = send(sock, buf, size, 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send");
					}

					state = RESULT_STATE;
					break;

				case 3:
					flag = true;
					break;
				}
				break;

			case RESULT_STATE:
				if (!UnPackingRecv(sock, buf))
				{
					break;
				}

				switch (GetProtocol(buf))
				{
					case JOIN_RESULT:
					case LOGIN_RESULT:
						ZeroMemory(msg, sizeof(msg));
						UnPacking(buf, result, msg);
						printf("%s\n", msg);
						break;
				}

				state = MENU_SELECT_STATE;
				break;
		}

	}

	closesocket(sock);
	WSACleanup();
	return 0;
}