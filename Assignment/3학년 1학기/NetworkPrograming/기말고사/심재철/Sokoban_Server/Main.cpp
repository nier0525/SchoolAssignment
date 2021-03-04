#include "PackingFunction.h"

// 접속 중인 클라이언트 인원 리스트
ClientInfo* ClientList[100];

// 접속 중인 클라이언트 인원 수
int ClientCount = 0;

// 크리티컬 섹션
CRITICAL_SECTION cs;

// 맵 정보
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

// 클라이언트 추가
ClientInfo* AddClientInfo(SOCKET sock, SOCKADDR_IN addr)
{
	// 클라이언트 인원수가 정해진 인원보다 찼다면 nullptr
	// 그렇지 않다면 클라이언트 구조체 내부 멤버 변수들 초기화 작업 후
	// 클라이언트 리스트에 추가해준다.
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

	printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));

	return ptr;
}

// 클라이언트 제거
void RemoveClientInfo(ClientInfo* _ptr)
{
	printf("\n[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(_ptr->addr.sin_addr), ntohs(_ptr->addr.sin_port));

	// 접속을 종료한 클라이언트를 리스트를 순회하여 찾은 후,
	// 클라이언트 정보를 삭제하고 배열에 빈 공간을 채워주는 작업 진행
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

// 게임 상대 찾기
bool MatchPartner(ClientInfo* ptr)
{
	EnterCriticalSection(&cs);
	for (int i = 0; i < ClientCount; i++)
	{
		// 나 자신이 아니고, 게임 상대가 없는 유저를 찾는다.
		if (ptr != ClientList[i] && ClientList[i]->part == nullptr)
		{
			// 상대와 나의 게임 상대를 서로 연결해준다.
			ClientList[i]->part = ptr;
			ptr->part = ClientList[i];
			LeaveCriticalSection(&cs);
			return true;
		}
	}
	LeaveCriticalSection(&cs);
	return false;
}

// 게임을 진행할 맵 세팅 작업
void InitStage(ClientInfo* ptr)
{
	EnterCriticalSection(&cs);
	// 현재 스테이지의 맵 정보를 자신의 게임 정보에 저장
	memcpy(ptr->user.Map, arStage[ptr->user.Stage], sizeof(ptr->user.Map));
	
	// 플레이어 위치를 찾는 작업
	// 찾은 후에 현재 위치를 게임 정보에 저장
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

// 플레이어 이동
void Move(ClientInfo* ptr, int x, int y)
{
	int nx = ptr->user.x;
	int ny = ptr->user.y;


	EnterCriticalSection(&cs);
	// '#' 은 벽, 'O' 은 상자, '.' 은 목표지점 ,공백은 빈 공간이다.
	if (ptr->user.Map[ny + y][nx + x] != '#')
	{
		if (ptr->user.Map[ny + y][nx + x] == 'O')
		{
			if (ptr->user.Map[ny + y * 2][nx + x * 2] == ' ' || ptr->user.Map[ny + y * 2][nx + x * 2] == '.')
			{
				// 자신이 지나 온 자리를 다시 본래 오브젝트로 바꿔주는 작업
				if (arStage[ptr->user.Stage][ny + y][nx + x] == '.')
				{
					ptr->user.Map[ny + y][nx + x] = '.';
				}
				else
				{
					ptr->user.Map[ny + y][nx + x] = ' ';
				}
				// 상자 밀기
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

// 게임 결과
bool GameResult(ClientInfo* ptr)
{
	EnterCriticalSection(&cs);
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH - 1; x++)
		{
			// 모든 상자를 목표 지점에 이동 시켰을 경우를 체크
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

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	// 입출력 완료 포트 생성
	HANDLE hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (hcp == NULL) return 1;

	// CPU 개수 확인
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	// (CPU 개수 * 2)개의 작업자 스레드 생성
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

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	DWORD recvbytes, flags;

	// 난수 생성을 위한 srand
	srand((unsigned)time(nullptr));

	while (1)
	{
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// 소켓과 입출력 완료 포트 연결
		CreateIoCompletionPort((HANDLE)client_sock, hcp, client_sock, 3);

		// 클라이언트 추가
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

// 작업 스레드
DWORD WINAPI WorkerThread(LPVOID arg)
{
	int retval;
	HANDLE hcp = (HANDLE)arg;

	while (true)
	{
		DWORD cbTransferrd;
		SOCKET client_sock;
		WSAOverLapped_EX* overlapped;

		// 송수신 입력 대기. 큐에서 순차적으로 받아온 후, 정보를 Overlapped_EX 에 저장한다.
		retval = GetQueuedCompletionStatus(hcp, &cbTransferrd, (LPDWORD)&client_sock, (LPOVERLAPPED*)&overlapped, INFINITE);
		// 받은 정보로 클라이언트 정보를 얻는다.
		ClientInfo* ptr = overlapped->ptr;

		// 에러 혹은 플레이어가 나갔을 경우
		if (retval == 0 || cbTransferrd == 0)
		{
			// 에러라면 정확한 에러를 출력해줌
			if (retval == 0)
			{
				DWORD temp1, temp2;
				WSAGetOverlappedResult(ptr->sock, &overlapped->overlapped, &temp1, false, &temp2);
				err_display("WSAGetOverlappedResult()");
			}
			// 접속 종료 상태로 변경
			ptr->state = DISCONNECTED_STATE;
		}

		// 접속 종료 상태가 아니라면
		if (ptr->state != DISCONNECTED_STATE)
		{
			// oerlapped 타입에 따라서 작업 분류
			switch (overlapped->type)
			{
			// 수신 타입
			case IO_TYPE::IO_RECV:
			{
				// 수신을 완전히 다 받았는 지 확인
				int result = CompleteRecv(ptr, cbTransferrd);
				switch (result)
				{
				case SOC_ERROR:
				case SOC_FALSE:
					continue;
				case SOC_TRUE:
					break;
				}

				// 받은 수신 데이터로 작업
				RecvProcess(ptr);

				if (!Recv(ptr))
					ptr->state = DISCONNECTED_STATE;
			}
			break;

			// 송신 타입
			case IO_TYPE::IO_SEND:
			{ 
				// 송신을 완전히 다 받았는 지 확인
				int result = CompleteSend(ptr, cbTransferrd);
				switch (result)
				{
				case SOC_ERROR:
				case SOC_FALSE:
					continue;
				case SOC_TRUE:
					break;
				}

				// 받은 송신 데이터로 작업
				// 이지만 여기서는 딱히 하는 일이 없다.
				SendProcess(ptr);
			}
			break;
			}
		}
		
		// 접속이 끊어졌을 경우
		if (ptr->state == DISCONNECTED_STATE)
		{
			// 게임 상대가 있는 상태에서 끊어진 경우
			// 즉 게임 중에 나간 상태라면
			if (ptr->part != nullptr)
			{
				// 게임 상대의 정보를 다시 매치를 기다리도록 갱신해준다.
				int size;
				PackPacket(ptr->part->sendbuf, REMATCH, size);
				if (!Send(ptr->part, size))
					ptr->part->state = DISCONNECTED_STATE;

				ptr->part->part = nullptr;
				ptr->part->state = SEND_RESULT_STATE;
			}

			// 클라이언트 정보 삭제
			RemoveClientInfo(ptr);
		}
	}
	return 0;
}

// 수신 작업
void RecvProcess(ClientInfo* ptr)
{
	int retval;
	int size;

	switch (ptr->state)
	{
	// 게임 상태
	case GAME_STATE:
		// 프로토콜에 따라 작업
		switch (GetProtocol(ptr->recvbuf))
		{
		// 처음에 접속한 상태
		case JOIN:
			// 게임 상대를 찾는다.
			if (!MatchPartner(ptr))
			{
				// 찾지 못했다면 위에서 대기
				printf("\n매칭 대기중\n");
				return;
			}

			EnterCriticalSection(&cs);
			// 송수신을 대기하고 있는 상대를 깨워준다.
			PackPacket(ptr->part->sendbuf, MATCH, size);
			if (!Send(ptr->part, size))
				ptr->part->state = DISCONNECTED_STATE;

			ptr->part->state = GAME_STATE;

			// 매치가 됬다고 송신해줌
			PackPacket(ptr->sendbuf, MATCH, size);
			if (!Send(ptr, size))
				ptr->state = DISCONNECTED_STATE;

			ptr->state = GAME_STATE;

			printf("\n매칭 성공\n");
			{
				// 랜덤으로 스테이지를 나오게 하기 위한 작업
				int index = rand() % 3;
				ptr->user.Stage = index;
				ptr->part->user.Stage = index;
			}
			LeaveCriticalSection(&cs);

			break;

		// 게임 정보 세팅
		case SETTING:			
			EnterCriticalSection(&cs);
			// 게임 정보 초기화
			InitStage(ptr);

			// 상대 정보를 초기화 해준다.
			// 중간중간 송신이 드랍되는 상황이 발생하는 것 같아서 상대 정보를 같이 보내주도록 하였다.
			PackPacket(ptr->part->sendbuf, MAPSETTING, ptr->part->user.Map, ptr->user.Map, ptr->part->user.Stage, ptr->user.Stage, ptr->part->user.x, ptr->part->user.y, ptr->user.x, ptr->user.y, size);
			if (!Send(ptr->part, size))
				ptr->state = DISCONNECTED_STATE;

			ptr->part->state = GAME_STATE;

			// 내 정보를 초기화 한다.
			PackPacket(ptr->sendbuf, MAPSETTING, ptr->user.Map, ptr->part->user.Map, ptr->user.Stage, ptr->part->user.Stage, ptr->user.x, ptr->user.y, ptr->part->user.x, ptr->part->user.y, size);
			if (!Send(ptr, size))
				ptr->state = DISCONNECTED_STATE;

			ptr->state = GAME_STATE;
			LeaveCriticalSection(&cs);
			break;

		// 클리어가 불가능한 상황일 때, 게임 다시 하기
		case RESET:
			EnterCriticalSection(&cs);
			// 게임 정보 초기화
			InitStage(ptr);

			// 상대의 렌더링 정보를 업데이트 해준다.
			PackPacket(ptr->part->sendbuf, UPDATE, ptr->part->user.Map, ptr->user.Map, ptr->part->user.Stage, ptr->user.Stage, ptr->part->user.x, ptr->part->user.y, ptr->user.x, ptr->user.y, size);
			if (!Send(ptr->part, size))
				ptr->state = DISCONNECTED_STATE;

			ptr->part->state = GAME_STATE;

			// 내 렌더링 정보를 업데이트 한다.
			PackPacket(ptr->sendbuf, UPDATE, ptr->user.Map, ptr->part->user.Map, ptr->user.Stage, ptr->part->user.Stage, ptr->user.x, ptr->user.y, ptr->part->user.x, ptr->part->user.y, size);
			if (!Send(ptr, size))
				ptr->state = DISCONNECTED_STATE;

			ptr->state = GAME_STATE;
			LeaveCriticalSection(&cs);
			break;

		case MOVE:
			EnterCriticalSection(&cs);
			int x, y;
			// 방향키에 따른 이동값을 언패킹하여 받아온다.
			UnPackPacket(ptr->recvbuf, x, y);
			// 이동 처리
			Move(ptr, x, y);

			// 만약 자신이 게임 승리 조건을 충족하였을 경우
			if (GameResult(ptr))
			{
				// 상대에겐 패배하였다는 정보를 송신한다.
				PackPacket(ptr->part->sendbuf, LOSE, size);
				if (!Send(ptr->part, size))
					ptr->state = DISCONNECTED_STATE;

				ptr->part->state = GAME_STATE;

				// 자신은 승리했다는 정보를 송신한다.
				PackPacket(ptr->sendbuf, WIN, size);
				if (!Send(ptr, size))
					ptr->state = DISCONNECTED_STATE;

				// 게임이 끝났기 때문에 게임 상대를 다시 nullptr 로 해준다.
				ptr->part = nullptr;
				ptr->state = GAME_STATE;
				LeaveCriticalSection(&cs);
				break;
			}

			// 상대 렌더링을 업데이트 해준다.
			PackPacket(ptr->part->sendbuf, UPDATE, ptr->part->user.Map, ptr->user.Map, ptr->part->user.Stage, ptr->user.Stage, ptr->part->user.x, ptr->part->user.y, ptr->user.x, ptr->user.y, size);
			if (!Send(ptr->part, size))
				ptr->state = DISCONNECTED_STATE;

			ptr->part->state = GAME_STATE;

			// 자신의 렌더링을 업데이트 한다.
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

// 송신 작업
void SendProcess(ClientInfo* ptr)
{
	switch (ptr->state)
	{
	case SEND_RESULT_STATE:
		ptr->state = GAME_STATE;
		break;
	}
}