#include "PackingFunction.h"

// ���� ���� Ŭ���̾�Ʈ �ο� ����Ʈ
ClientInfo* ClientList[100];

// ���� ���� Ŭ���̾�Ʈ �ο� ��
int ClientCount = 0;

// ũ��Ƽ�� ����
CRITICAL_SECTION cs;

// �� ����
char arStage[MAXSTAGE][18][21] = {
	{
	"####################",
	"####################",
	"####################",
	"#####   ############",
	"#####O  ############",
	"#####  O############",
	"###  O O ###########",
	"### # ## ###########",
	"#   # ## #####  ..##",
	"# O  O   @      ..##",
	"##### ### # ##  ..##",
	"#####     ##########",
	"####################",
	"####################",
	"####################",
	"####################",
	"####################",
	"####################"
	},
	{
	"####################",
	"####################",
	"####################",
	"####################",
	"####..  #     ######",
	"####..  # O  O  ####",
	"####..  #O####  ####",
	"####..    @ ##  ####",
	"####..  # #  O #####",
	"######### ##O O ####",
	"###### O  O O O ####",
	"######    #     ####",
	"####################",
	"####################",
	"####################",
	"####################",
	"####################",
	"####################"
	},
	{
	"####################",
	"####################",
	"####################",
	"####################",
	"##########     @####",
	"########## O#O #####",
	"########## O  O#####",
	"###########O O #####",
	"########## O # #####",
	"##....  ## O  O  ###",
	"###...    O  O   ###",
	"##....  ############",
	"####################",
	"####################",
	"####################",
	"####################",
	"####################",
	"####################"
	},
};

DWORD WINAPI WorkerThread(LPVOID arg);
void RecvProcess(ClientInfo* ptr);
void SendProcess(ClientInfo* ptr);

// Ŭ���̾�Ʈ �߰�
ClientInfo* AddClientInfo(SOCKET sock, SOCKADDR_IN addr)
{
	// Ŭ���̾�Ʈ �ο����� ������ �ο����� á�ٸ� nullptr
	// �׷��� �ʴٸ� Ŭ���̾�Ʈ ����ü ���� ��� ������ �ʱ�ȭ �۾� ��
	// Ŭ���̾�Ʈ ����Ʈ�� �߰����ش�.
	if (ClientCount >= WSA_MAXIMUM_WAIT_EVENTS) return nullptr;

	EnterCriticalSection(&cs);

	ClientInfo* ptr = new ClientInfo();
	ZeroMemory(ptr, sizeof(ClientInfo));
	ptr->sock = sock;
	memcpy(&ptr->addr, &addr, sizeof(SOCKADDR_IN));
	ptr->r_sizeflag = false;

	ptr->r_overlapped.ptr = ptr;
	ptr->r_overlapped.type = IO_TYPE::IO_RECV;

	ptr->s_overlapped.ptr = ptr;
	ptr->s_overlapped.type = IO_TYPE::IO_SEND;

	ptr->state = GAME_STATE;

	ptr->user.Stage = 0;
	memcpy(ptr->user.Map, arStage[ptr->user.Stage], sizeof(ptr->user.Map));

	ptr->part = nullptr;
	ClientList[ClientCount] = ptr;
	ClientCount++;

	LeaveCriticalSection(&cs);

	printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));

	return ptr;
}

// Ŭ���̾�Ʈ ����
void RemoveClientInfo(ClientInfo* _ptr)
{
	printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		inet_ntoa(_ptr->addr.sin_addr), ntohs(_ptr->addr.sin_port));

	// ������ ������ Ŭ���̾�Ʈ�� ����Ʈ�� ��ȸ�Ͽ� ã�� ��,
	// Ŭ���̾�Ʈ ������ �����ϰ� �迭�� �� ������ ä���ִ� �۾� ����
	EnterCriticalSection(&cs);
	for (int i = 0; i < ClientCount; i++)
	{
		if (ClientList[i] == _ptr)
		{
			closesocket(ClientList[i]->sock);
			ClientList[i]->part = nullptr;
			delete ClientList[i];

			for (int j = i; j < ClientCount - 1; j++)
				ClientList[j] = ClientList[j + 1];

			break;
		}
	}

	ClientCount--;
	LeaveCriticalSection(&cs);
}

// ���� ��� ã��
bool MatchPartner(ClientInfo* ptr)
{
	EnterCriticalSection(&cs);
	for (int i = 0; i < ClientCount; i++)
	{
		// �� �ڽ��� �ƴϰ�, ���� ��밡 ���� ������ ã�´�.
		if (ptr != ClientList[i] && ClientList[i]->part == nullptr)
		{
			// ���� ���� ���� ��븦 ���� �������ش�.
			ClientList[i]->part = ptr;
			ptr->part = ClientList[i];
			LeaveCriticalSection(&cs);
			return true;
		}
	}
	LeaveCriticalSection(&cs);
	return false;
}

// ������ ������ �� ���� �۾�
void InitStage(ClientInfo* ptr)
{
	EnterCriticalSection(&cs);
	// ���� ���������� �� ������ �ڽ��� ���� ������ ����
	memcpy(ptr->user.Map, arStage[ptr->user.Stage], sizeof(ptr->user.Map));
	
	// �÷��̾� ��ġ�� ã�� �۾�
	// ã�� �Ŀ� ���� ��ġ�� ���� ������ ����
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH - 1; x++)
		{
			if (ptr->user.Map[y][x] == '@')
			{
				ptr->user.x = x;
				ptr->user.y = y;
				ptr->user.Map[y][x] = ' ';

				LeaveCriticalSection(&cs);
				return;
			}
		}
	}
	LeaveCriticalSection(&cs);
}

// �÷��̾� �̵�
void Move(ClientInfo* ptr, int x, int y)
{
	int nx = ptr->user.x;
	int ny = ptr->user.y;


	EnterCriticalSection(&cs);
	// '#' �� ��, 'O' �� ����, '.' �� ��ǥ���� ,������ �� �����̴�.
	if (ptr->user.Map[ny + y][nx + x] != '#')
	{
		if (ptr->user.Map[ny + y][nx + x] == 'O')
		{
			if (ptr->user.Map[ny + y * 2][nx + x * 2] == ' ' || ptr->user.Map[ny + y * 2][nx + x * 2] == '.')
			{
				// �ڽ��� ���� �� �ڸ��� �ٽ� ���� ������Ʈ�� �ٲ��ִ� �۾�
				if (arStage[ptr->user.Stage][ny + y][nx + x] == '.')
				{
					ptr->user.Map[ny + y][nx + x] = '.';
				}
				else
				{
					ptr->user.Map[ny + y][nx + x] = ' ';
				}
				// ���� �б�
				ptr->user.Map[ny + y * 2][nx + x * 2] = 'O';
			}
			else
			{
				LeaveCriticalSection(&cs);
				return;
			}
		}

		ptr->user.x += x;
		ptr->user.y += y;
	}
	LeaveCriticalSection(&cs);
}

// ���� ���
bool GameResult(ClientInfo* ptr)
{
	EnterCriticalSection(&cs);
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH - 1; x++)
		{
			// ��� ���ڸ� ��ǥ ������ �̵� ������ ��츦 üũ
			if (arStage[ptr->user.Stage][y][x] == '.' && ptr->user.Map[y][x] != 'O')
			{
				LeaveCriticalSection(&cs);
				return false;
			}
		}
	}
	LeaveCriticalSection(&cs);
	return true;
}

int main()
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	// ����� �Ϸ� ��Ʈ ����
	HANDLE hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (hcp == NULL) return 1;

	// CPU ���� Ȯ��
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	// (CPU ���� * 2)���� �۾��� ������ ����
	HANDLE hThread;
	for (int i = 0; i < (int)si.dwNumberOfProcessors * 2; i++) {
		hThread = CreateThread(NULL, 0, WorkerThread, hcp, 0, NULL);
		if (hThread == NULL) return 1;
		CloseHandle(hThread);
	}

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(PORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	InitializeCriticalSection(&cs);

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	DWORD recvbytes, flags;

	// ���� ������ ���� srand
	srand((unsigned)time(nullptr));

	while (1)
	{
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// ���ϰ� ����� �Ϸ� ��Ʈ ����
		CreateIoCompletionPort((HANDLE)client_sock, hcp, client_sock, 3);

		// Ŭ���̾�Ʈ �߰�
		ClientInfo* ptr = AddClientInfo(client_sock, clientaddr);

		if (!Recv(ptr))
		{
			RemoveClientInfo(ptr);
			continue;
		}
	}

	DeleteCriticalSection(&cs);

	closesocket(listen_sock);
	WSACleanup();
	return 0;
}

// �۾� ������
DWORD WINAPI WorkerThread(LPVOID arg)
{
	int retval;
	HANDLE hcp = (HANDLE)arg;

	while (true)
	{
		DWORD cbTransferrd;
		SOCKET client_sock;
		WSAOverLapped_EX* overlapped;

		// �ۼ��� �Է� ���. ť���� ���������� �޾ƿ� ��, ������ Overlapped_EX �� �����Ѵ�.
		retval = GetQueuedCompletionStatus(hcp, &cbTransferrd, (LPDWORD)&client_sock, (LPOVERLAPPED*)&overlapped, INFINITE);
		// ���� ������ Ŭ���̾�Ʈ ������ ��´�.
		ClientInfo* ptr = overlapped->ptr;

		// ���� Ȥ�� �÷��̾ ������ ���
		if (retval == 0 || cbTransferrd == 0)
		{
			// ������� ��Ȯ�� ������ �������
			if (retval == 0)
			{
				DWORD temp1, temp2;
				WSAGetOverlappedResult(ptr->sock, &overlapped->overlapped, &temp1, false, &temp2);
				err_display("WSAGetOverlappedResult()");
			}
			// ���� ���� ���·� ����
			ptr->state = DISCONNECTED_STATE;
		}

		// ���� ���� ���°� �ƴ϶��
		if (ptr->state != DISCONNECTED_STATE)
		{
			// oerlapped Ÿ�Կ� ���� �۾� �з�
			switch (overlapped->type)
			{
			// ���� Ÿ��
			case IO_TYPE::IO_RECV:
			{
				// ������ ������ �� �޾Ҵ� �� Ȯ��
				int result = CompleteRecv(ptr, cbTransferrd);
				switch (result)
				{
				case SOC_ERROR:
				case SOC_FALSE:
					continue;
				case SOC_TRUE:
					break;
				}

				// ���� ���� �����ͷ� �۾�
				RecvProcess(ptr);

				if (!Recv(ptr))
					ptr->state = DISCONNECTED_STATE;
			}
			break;

			// �۽� Ÿ��
			case IO_TYPE::IO_SEND:
			{ 
				// �۽��� ������ �� �޾Ҵ� �� Ȯ��
				int result = CompleteSend(ptr, cbTransferrd);
				switch (result)
				{
				case SOC_ERROR:
				case SOC_FALSE:
					continue;
				case SOC_TRUE:
					break;
				}

				// ���� �۽� �����ͷ� �۾�
				// ������ ���⼭�� ���� �ϴ� ���� ����.
				SendProcess(ptr);
			}
			break;
			}
		}
		
		// ������ �������� ���
		if (ptr->state == DISCONNECTED_STATE)
		{
			// ���� ��밡 �ִ� ���¿��� ������ ���
			// �� ���� �߿� ���� ���¶��
			if (ptr->part != nullptr)
			{
				// ���� ����� ������ �ٽ� ��ġ�� ��ٸ����� �������ش�.
				int size;
				PackPacket(ptr->part->sendbuf, REMATCH, size);
				if (!Send(ptr->part, size))
					ptr->part->state = DISCONNECTED_STATE;

				ptr->part->part = nullptr;
				ptr->part->state = SEND_RESULT_STATE;
			}

			// Ŭ���̾�Ʈ ���� ����
			RemoveClientInfo(ptr);
		}
	}
	return 0;
}

// ���� �۾�
void RecvProcess(ClientInfo* ptr)
{
	int retval;
	int size;

	switch (ptr->state)
	{
	// ���� ����
	case GAME_STATE:
		// �������ݿ� ���� �۾�
		switch (GetProtocol(ptr->recvbuf))
		{
		// ó���� ������ ����
		case JOIN:
			// ���� ��븦 ã�´�.
			if (!MatchPartner(ptr))
			{
				// ã�� ���ߴٸ� ������ ���
				printf("\n��Ī �����\n");
				return;
			}

			EnterCriticalSection(&cs);
			// �ۼ����� ����ϰ� �ִ� ��븦 �����ش�.
			PackPacket(ptr->part->sendbuf, MATCH, size);
			if (!Send(ptr->part, size))
				ptr->part->state = DISCONNECTED_STATE;

			ptr->part->state = GAME_STATE;

			// ��ġ�� ��ٰ� �۽�����
			PackPacket(ptr->sendbuf, MATCH, size);
			if (!Send(ptr, size))
				ptr->state = DISCONNECTED_STATE;

			ptr->state = GAME_STATE;

			printf("\n��Ī ����\n");
			{
				// �������� ���������� ������ �ϱ� ���� �۾�
				int index = rand() % 3;
				ptr->user.Stage = index;
				ptr->part->user.Stage = index;
			}
			LeaveCriticalSection(&cs);

			break;

		// ���� ���� ����
		case SETTING:			
			EnterCriticalSection(&cs);
			// ���� ���� �ʱ�ȭ
			InitStage(ptr);

			// ��� ������ �ʱ�ȭ ���ش�.
			// �߰��߰� �۽��� ����Ǵ� ��Ȳ�� �߻��ϴ� �� ���Ƽ� ��� ������ ���� �����ֵ��� �Ͽ���.
			PackPacket(ptr->part->sendbuf, MAPSETTING, ptr->part->user.Map, ptr->user.Map, ptr->part->user.Stage, ptr->user.Stage, ptr->part->user.x, ptr->part->user.y, ptr->user.x, ptr->user.y, size);
			if (!Send(ptr->part, size))
				ptr->state = DISCONNECTED_STATE;

			ptr->part->state = GAME_STATE;

			// �� ������ �ʱ�ȭ �Ѵ�.
			PackPacket(ptr->sendbuf, MAPSETTING, ptr->user.Map, ptr->part->user.Map, ptr->user.Stage, ptr->part->user.Stage, ptr->user.x, ptr->user.y, ptr->part->user.x, ptr->part->user.y, size);
			if (!Send(ptr, size))
				ptr->state = DISCONNECTED_STATE;

			ptr->state = GAME_STATE;
			LeaveCriticalSection(&cs);
			break;

		// Ŭ��� �Ұ����� ��Ȳ�� ��, ���� �ٽ� �ϱ�
		case RESET:
			EnterCriticalSection(&cs);
			// ���� ���� �ʱ�ȭ
			InitStage(ptr);

			// ����� ������ ������ ������Ʈ ���ش�.
			PackPacket(ptr->part->sendbuf, UPDATE, ptr->part->user.Map, ptr->user.Map, ptr->part->user.Stage, ptr->user.Stage, ptr->part->user.x, ptr->part->user.y, ptr->user.x, ptr->user.y, size);
			if (!Send(ptr->part, size))
				ptr->state = DISCONNECTED_STATE;

			ptr->part->state = GAME_STATE;

			// �� ������ ������ ������Ʈ �Ѵ�.
			PackPacket(ptr->sendbuf, UPDATE, ptr->user.Map, ptr->part->user.Map, ptr->user.Stage, ptr->part->user.Stage, ptr->user.x, ptr->user.y, ptr->part->user.x, ptr->part->user.y, size);
			if (!Send(ptr, size))
				ptr->state = DISCONNECTED_STATE;

			ptr->state = GAME_STATE;
			LeaveCriticalSection(&cs);
			break;

		case MOVE:
			EnterCriticalSection(&cs);
			int x, y;
			// ����Ű�� ���� �̵����� ����ŷ�Ͽ� �޾ƿ´�.
			UnPackPacket(ptr->recvbuf, x, y);
			// �̵� ó��
			Move(ptr, x, y);

			// ���� �ڽ��� ���� �¸� ������ �����Ͽ��� ���
			if (GameResult(ptr))
			{
				// ��뿡�� �й��Ͽ��ٴ� ������ �۽��Ѵ�.
				PackPacket(ptr->part->sendbuf, LOSE, size);
				if (!Send(ptr->part, size))
					ptr->state = DISCONNECTED_STATE;

				ptr->part->state = GAME_STATE;

				// �ڽ��� �¸��ߴٴ� ������ �۽��Ѵ�.
				PackPacket(ptr->sendbuf, WIN, size);
				if (!Send(ptr, size))
					ptr->state = DISCONNECTED_STATE;

				// ������ ������ ������ ���� ��븦 �ٽ� nullptr �� ���ش�.
				ptr->part = nullptr;
				ptr->state = GAME_STATE;
				LeaveCriticalSection(&cs);
				break;
			}

			// ��� �������� ������Ʈ ���ش�.
			PackPacket(ptr->part->sendbuf, UPDATE, ptr->part->user.Map, ptr->user.Map, ptr->part->user.Stage, ptr->user.Stage, ptr->part->user.x, ptr->part->user.y, ptr->user.x, ptr->user.y, size);
			if (!Send(ptr->part, size))
				ptr->state = DISCONNECTED_STATE;

			ptr->part->state = GAME_STATE;

			// �ڽ��� �������� ������Ʈ �Ѵ�.
			PackPacket(ptr->sendbuf, UPDATE, ptr->user.Map, ptr->part->user.Map, ptr->user.Stage, ptr->part->user.Stage, ptr->user.x, ptr->user.y, ptr->part->user.x, ptr->part->user.y, size);
			if (!Send(ptr, size))
				ptr->state = DISCONNECTED_STATE;

			ptr->state = GAME_STATE;
			LeaveCriticalSection(&cs);

			break;

		}
		break;
	}
}

// �۽� �۾�
void SendProcess(ClientInfo* ptr)
{
	switch (ptr->state)
	{
	case SEND_RESULT_STATE:
		ptr->state = GAME_STATE;
		break;
	}
}