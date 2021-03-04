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

	char buf[BUFSIZE + 1];
	int len;

	char msg[MSGSIZE];
	int size;
	bool flag = false;

	UserInfo user;
	ZeroMemory(&user, sizeof(user));
	PROTOCOL protocol = INIT;
	RESULT result;

	size = Packing(buf, protocol);
	retval = send(sock, buf, size, 0);
	if (retval == SOCKET_ERROR)
		err_display("send()");

	while (!flag)
	{
		if (!UnPackingRecv(sock, buf))
		{
			break;
		}

		protocol = GetProtocol(buf);

		switch (protocol)
		{
		case INTRO:
			ZeroMemory(msg, sizeof(msg));
			UnPacking(buf, msg);
			printf("%s", msg);

			ZeroMemory(msg, sizeof(msg));
			scanf("%s", msg);

			switch (atoi(msg))
			{
			case 1:
				protocol = JOIN;
				break;

			case 2:
				protocol = LOGIN;
				break;

			case 3:
				protocol = EXIT;
				flag = true;
				break;
			}

			size = Packing(buf, protocol);
			retval = send(sock, buf, size, 0);
			if (retval == SOCKET_ERROR)
				err_display("send()");
			break;

		case ID_PW_INFO:
			ZeroMemory(&user, sizeof(user));
			UnPacking(buf, user.id, user.pw);

			printf("%s", user.id);
			scanf("%s", user.id);
			printf("%s", user.pw);
			scanf("%s", user.pw);

			switch (atoi(msg))
			{
			case 1:
				size = Packing(buf, JOIN_INFO, user.id, user.pw);
				break;

			case 2:
				size = Packing(buf, LOGIN_INFO, user.id, user.pw);
				break;
			}

			size = Packing(buf, protocol, user.id, user.pw);
			retval = send(sock, buf, size, 0);
			if (retval == SOCKET_ERROR)
				err_display("send()");
			break;


		case JOIN_RESULT:
		case LOGIN_RESULT:
			ZeroMemory(msg, sizeof(msg));
			UnPacking(buf, result, msg);
			printf("%s", msg);
			break;

		}

	}

	closesocket(sock);
	WSACleanup();
	return 0;
}