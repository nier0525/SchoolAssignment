#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 9000
#define BUFSIZE    4092
#define MSGSIZE 128
#define USERSIZE 100
#define NONE 0

#define ERROR_DISCONNECTED -2
#define DISCONNECTED -1
#define SOCKET_TRUE 1
#define SOCKET_FALSE 0

#define FILE_PATH "UserInfo.info"

#define INTRO_MSG "\n1. 회원가입\n2. 로그인\n3. 종료\n\n입력 : "
#define LOGIN_INTRO_MSG "\n1. 로그아웃\n2. 회원탈퇴\n\n입력 : "
#define ID_MSG "ID : "
#define PW_MSG "PW : "
#define NEW_MSG "\n정상적으로 회원가입 되었습니다.\n"
#define NEW_ERROR_MSG "\n중복된 아이디입니다.\n"
#define LOGIN_MSG "\n정상적으로 로그인 되었습니다.\n"
#define ID_ERROR_MSG "\n없는 ID 입니다.\n"
#define PW_ERROR_MSG "\nPW 가 틀렸습니다.\n"
#define OVERRAP_ERROR_MSG "\n접속 중인 계정입니다.\n"
#define LOGOUT_MSG "\n정상적으로 로그아웃 되었습니다.\n"
#define REMOVE_MSG "\n회원탈퇴 되었습니다.\n"

enum PROTOCOL
{
	INTRO = 1,
	INTRO_SELECT_MENU,
	LOGIN_SELECT_MENU,
	ID_PW_MSG,
	NEW_INFO,
	LOGIN_INFO,
	NEW_RESULT,
	LOGIN_RESULT,
	LOGIN_INTRO,
	LOGOUT,
	REMOVE
};

enum STATE
{
	INTRO_STATE = 1,
	MENU_SELECT_STATE,
	INFO_STATE,
	NEW_INFO_STATE,
	USER_INFO_STATE,
	NEW_STATE,
	LOGIN_STATE,
	LOGIN_INTRO_STATE,
	LOGIN_SELECT_STATE,
	LOGOUT_STATE,
	REMOVE_STATE,
	DISCONNECTED_STATE
};

enum RESULT
{
	NODATA = -1,
	ID_EXIST = 1,
	ID_ERROR,
	PW_ERROR,
	LOGIN_ERROR,
	NEW_SUCCESS,
	LOGIN_SUCCESS,
};

enum INTRO_MENU
{
	NEW_MENU = 1,
	LOGIN_MENU,
	EXIT_MENU
};

enum LOGIN_MENU
{
	LOGOUT_MENU = 1,
	REMOVE_MENU,
};

struct User_Info
{
	char id[128];
	char pw[128];

	bool state;
};

// 소켓 정보 저장을 위한 구조체와 변수
struct SOCKETINFO
{
	SOCKET sock;
	SOCKADDR_IN addr;

	char recv_buf[BUFSIZE + 1];
	char send_buf[BUFSIZE + 1];

	int total_recvbytes;
	int total_sendbytes;

	int sendbytes;
	int recvbytes;

	User_Info user;
	STATE state;

	char msg[MSGSIZE];
	bool flag;
};

int nTotalSockets = 0;
SOCKETINFO* SocketInfoArray[FD_SETSIZE];

int usercount = 0;
User_Info userlist[USERSIZE];

// 소켓 초기화
SOCKET ListenSocket_Init();

// 소켓 관리 함수
BOOL AddSocketInfo(SOCKET sock, SOCKADDR_IN addr);
void RemoveSocketInfo(SOCKETINFO* ptr, int& index);

// 오류 출력 함수
void err_quit(const char* msg);
void err_display(const char* msg);

PROTOCOL GetProtocol(char* buf) {
	PROTOCOL protocol;
	memcpy(&protocol, buf, sizeof(PROTOCOL));

	return protocol;
}

// 가변 길이 패킹 함수
int memcpy_send(void* ptr, const void* dst, size_t size, int* len) {
	memcpy(ptr, dst, size);	// 메모리 복사
	*len += size;						// 메모리의 크기를 인자값으로 받은 크기만큼 증가
	return size;						// 인자값으로 받은 크기 리턴
}

// 가변 길이 언패킹 함수
int memcpy_recv(void* dst, const void* ptr, size_t size) {
	memcpy(dst, ptr, size);	// 메모리 복사
	return size;						// 인자값으로 받은 크기 리턴
}

// Packing
int Packing(char* buf, PROTOCOL protocol, const char* str1) {
	int size = 0;
	char* ptr = buf + sizeof(int);
	int len = strlen(str1);

	ptr += memcpy_send(ptr, &protocol, sizeof(PROTOCOL), &size);

	ptr += memcpy_send(ptr, &len, sizeof(int), &size);
	ptr += memcpy_send(ptr, str1, len, &size);

	ptr = buf;
	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}

int Packing(char* buf, PROTOCOL protocol, RESULT result, const char* str1) {
	int size = 0;
	char* ptr = buf + sizeof(int);
	int len = strlen(str1);

	ptr += memcpy_send(ptr, &protocol, sizeof(PROTOCOL), &size);
	ptr += memcpy_send(ptr, &result, sizeof(RESULT), &size);

	ptr += memcpy_send(ptr, &len, sizeof(int), &size);
	ptr += memcpy_send(ptr, str1, len, &size);

	ptr = buf;
	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}

int Packing(char* buf, PROTOCOL protocol, const char* str1, const char* str2) {
	int size = 0;
	char* ptr = buf + sizeof(int);
	int len = strlen(str1);
	int len2 = strlen(str2);

	ptr += memcpy_send(ptr, &protocol, sizeof(PROTOCOL), &size);

	ptr += memcpy_send(ptr, &len, sizeof(int), &size);
	ptr += memcpy_send(ptr, str1, len, &size);
	ptr += memcpy_send(ptr, &len2, sizeof(int), &size);
	ptr += memcpy_send(ptr, str2, len2, &size);

	ptr = buf;
	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}

// UnPacking
void UnPacking(const char* buf, char* str) {
	const char* ptr = buf + sizeof(PROTOCOL);
	int len = 0;

	ptr += memcpy_recv(&len, ptr, sizeof(int));
	ptr += memcpy_recv(str, ptr, len);
}

void UnPacking(const char* buf, char* str, char* str2) {
	const char* ptr = buf + sizeof(PROTOCOL);
	int len = 0;
	int len2 = 0;

	ptr += memcpy_recv(&len, ptr, sizeof(int));
	ptr += memcpy_recv(str, ptr, len);
	ptr += memcpy_recv(&len2, ptr, sizeof(int));
	ptr += memcpy_recv(str2, ptr, len2);
}


// -------------------------------------------------------------------------------------

bool FileData_Search(const char* name)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFindFile = FindFirstFile(name, &FindFileData);
	if (hFindFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	else
	{
		FindClose(hFindFile);
		return true;
	}
}

bool FileData_Save(User_Info* user)
{
	FILE* data = fopen(FILE_PATH, "wb");
	if (data == NULL)
	{
		return false;
	}

	int retval = fwrite(user, sizeof(User_Info), usercount, data);
	if (retval != usercount)
	{
		printf("\n%d / %d\n", retval, sizeof(User_Info) * usercount);
		fclose(data);
		return false;
	}

	fclose(data);
	return true;
}

bool FileData_Load()
{
	if (!FileData_Search(FILE_PATH))
	{
		FILE* data = fopen(FILE_PATH, "wb");
		fclose(data);
		return true;
	}

	FILE* data = fopen(FILE_PATH, "rb");
	if (data == NULL)
	{
		return false;
	}

	User_Info info;
	ZeroMemory(&info, sizeof(User_Info));

	while (1)
	{
		fread(&info, sizeof(User_Info), 1, data);
		if (feof(data))
		{
			break;
		}
		User_Info ptr;
		memcpy(&ptr, &info, sizeof(User_Info));
		userlist[usercount++] = ptr;
	}

	fclose(data);
	return true;
}

// -------------------------------------------------------------------------------------

int Message_Recv(SOCKETINFO* ptr) 
{
	int retval = recv(ptr->sock, ptr->recv_buf + ptr->total_recvbytes, ptr->recvbytes - ptr->total_recvbytes, 0);
	if (retval == SOCKET_ERROR)
	{
		return ERROR_DISCONNECTED;
	}
	else if (retval == 0)
	{
		return DISCONNECTED;
	}
	else {
		ptr->total_recvbytes += retval;
		if (ptr->total_recvbytes == ptr->recvbytes) 
		{
			ptr->total_recvbytes = ptr->recvbytes = 0;
			return SOCKET_TRUE;
		}
		else
		{
			return SOCKET_FALSE;
		}
	}
}

int Message_Send(SOCKETINFO* ptr) 
{
	int retval = send(ptr->sock, ptr->send_buf + ptr->total_sendbytes, ptr->sendbytes - ptr->total_sendbytes, 0);
	if (retval == SOCKET_ERROR) 
	{
		return ERROR_DISCONNECTED;
	}
	else if (retval == 0)
	{
		return DISCONNECTED;
	}
	else
	{
		ptr->total_sendbytes += retval;
		if (ptr->sendbytes == ptr->total_sendbytes)
		{
			ptr->total_sendbytes = ptr->sendbytes = 0;
			return SOCKET_TRUE;
		}
		else
		{
			return SOCKET_FALSE;
		}
	}
}

int Packing_Recv(SOCKETINFO* ptr) 
{
	if (!ptr->flag) 
	{
		ptr->recvbytes = sizeof(int);
		int retval = Message_Recv(ptr);

		switch (retval)
		{
		case SOCKET_TRUE:
			memcpy(&ptr->recvbytes, ptr->recv_buf, sizeof(int));
			ptr->flag = true;
			return SOCKET_FALSE;

		case SOCKET_FALSE:
			return SOCKET_FALSE;

		case ERROR_DISCONNECTED:
			err_display("recv error()");
			return DISCONNECTED;

		case DISCONNECTED:
			return DISCONNECTED;
		}
	}

	else
	{
		int retval = Message_Recv(ptr);

		switch (retval)
		{
		case SOCKET_TRUE:
			ptr->flag = false;
			return SOCKET_TRUE;

		case SOCKET_FALSE:
			return SOCKET_FALSE;

		case ERROR_DISCONNECTED:
			err_display("recv error()");
			return DISCONNECTED;

		case DISCONNECTED:
			return DISCONNECTED;
		}
	}
}

int Result_Send(SOCKETINFO* ptr)
{
	int result = Message_Send(ptr);
	switch (result)
	{
	case ERROR_DISCONNECTED:
		err_display("connect end");
	case DISCONNECTED:
		ptr->state = DISCONNECTED_STATE;
		return DISCONNECTED;

	case SOCKET_FALSE:
		return SOCKET_FALSE;

	case SOCKET_TRUE:
		return SOCKET_TRUE;
	}
}

void Recv_PackingProcess(SOCKETINFO* ptr)
{
	PROTOCOL protocol;

	switch (ptr->state)
	{
	case MENU_SELECT_STATE:
		protocol = GetProtocol(ptr->recv_buf);

		ZeroMemory(ptr->msg, sizeof(MSGSIZE));
		UnPacking(ptr->recv_buf, ptr->msg);

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
		}
		break;

	case USER_INFO_STATE:
		protocol = GetProtocol(ptr->recv_buf);
		ZeroMemory(&ptr->user, sizeof(User_Info));
		UnPacking(ptr->recv_buf, ptr->user.id, ptr->user.pw);

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
	}
}

void Send_PackingProcess(SOCKETINFO* ptr)
{
	RESULT result = NODATA;
	PROTOCOL protocol;

	int retval;
	bool flag = false;

	switch (ptr->state)
	{
	case INTRO_STATE:
		ptr->sendbytes = Packing(ptr->send_buf, INTRO, INTRO_MSG);
		ptr->state = MENU_SELECT_STATE;
		break;

	case INFO_STATE:
		ptr->sendbytes = Packing(ptr->send_buf, ID_PW_MSG, ID_MSG, PW_MSG);
		ptr->state = USER_INFO_STATE;
		break;

	case NEW_STATE:
		flag = false;

		for (int i = 0; i < usercount; i++)
		{
			if (!strcmp(userlist[i].id, ptr->user.id))
			{
				ptr->sendbytes = Packing(ptr->send_buf, NEW_RESULT, ID_EXIST, NEW_ERROR_MSG);
				flag = true;
				break;
			}
		}

		if (!flag)
		{
			ptr->sendbytes = Packing(ptr->send_buf, NEW_RESULT, NEW_SUCCESS, NEW_MSG);
			userlist[usercount++] = ptr->user;

			if (FileData_Save(userlist))
			{
				printf("\n[FileData Save]\n");
			}
		}
		ptr->state = INTRO_STATE;
		break;

	case LOGIN_STATE:
		flag = false;

		for (int i = 0; i < usercount; i++)
		{
			if (!strcmp(userlist[i].id, ptr->user.id))
			{
				flag = true;
				if (!strcmp(userlist[i].pw, ptr->user.pw))
				{
					if (!userlist[i].state)
					{
						ptr->sendbytes = Packing(ptr->send_buf, LOGIN_RESULT, LOGIN_SUCCESS, LOGIN_MSG);
						ptr->state = LOGIN_INTRO_STATE;
						userlist[i].state = true;
					}
					else
					{
						ptr->sendbytes = Packing(ptr->send_buf, LOGIN_RESULT, LOGIN_ERROR, OVERRAP_ERROR_MSG);
						ptr->state = INTRO_STATE;
					}
				}
				else
				{
					ptr->sendbytes = Packing(ptr->send_buf, LOGIN_RESULT, PW_ERROR, PW_ERROR_MSG);
					ptr->state = INTRO_STATE;
				}
				break;
			}
		}

		if (!flag)
		{
			ptr->sendbytes = Packing(ptr->send_buf, LOGIN_RESULT, ID_ERROR, ID_ERROR_MSG);
			ptr->state = INTRO_STATE;
		}
		break;

	case LOGIN_INTRO_STATE:
		ptr->sendbytes = Packing(ptr->send_buf, LOGIN_INTRO, LOGIN_INTRO_MSG);
		ptr->state = MENU_SELECT_STATE;
		break;

	case LOGOUT_STATE:
		for (int i = 0; i < usercount; i++)
		{
			if (!strcmp(userlist[i].id, ptr->user.id))
			{
				userlist[i].state = false;
			}
		}

		ptr->sendbytes = Packing(ptr->send_buf, LOGOUT, LOGOUT_MSG);
		ptr->state = INTRO_STATE;
		break;

	case REMOVE_STATE:
		for (int i = 0; i < usercount; i++)
		{
			if (!strcmp(userlist[i].id, ptr->user.id))
			{
				for (int j = i; j < usercount - 1; j++)
				{
					userlist[j] = userlist[j + 1];
				}
				ZeroMemory(&userlist[usercount - 1], sizeof(User_Info));
				usercount -= 1;

				if (FileData_Save(userlist))
				{
					printf("\n[FileData Save]\n");
				}
				break;
			}
		}

		ptr->sendbytes = Packing(ptr->send_buf, REMOVE, REMOVE_MSG);
		ptr->state = INTRO_STATE;
		break;
	}
}

int main(int argc, char* argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET listen_sock = ListenSocket_Init();

	// 데이터 통신에 사용할 변수
	FD_SET rset, wset;
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen, index;

	if (FileData_Load())
	{
		printf("\n[FileData Load]\n");
	}

	while (1) {
		// 소켓 셋 초기화
		FD_ZERO(&rset);
		FD_ZERO(&wset);
		FD_SET(listen_sock, &rset);

		for (index = 0; index < nTotalSockets; index++)
		{
			FD_SET(SocketInfoArray[index]->sock, &rset);

			switch (SocketInfoArray[index]->state) 
			{
			case INTRO_STATE:
			case INFO_STATE:
			case NEW_STATE:
			case LOGIN_STATE:
			case LOGIN_INTRO_STATE:
			case LOGOUT_STATE:
			case REMOVE_STATE:
				FD_SET(SocketInfoArray[index]->sock, &wset);
				break;
			}
		}

		// select()
		retval = select(0, &rset, &wset, NULL, NULL);
		if (retval == SOCKET_ERROR) err_quit("select()");

		// 소켓 셋 검사(1): 클라이언트 접속 수용
		if (FD_ISSET(listen_sock, &rset)) 
		{
			addrlen = sizeof(clientaddr);
			client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
			if (client_sock == INVALID_SOCKET) 
			{
				err_display("accept()");
			}
			else
			{
				printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
					inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
				// 소켓 정보 추가
				AddSocketInfo(client_sock, clientaddr);
			}
		}

		// 소켓 셋 검사(2): 데이터 통신
		for (index = 0; index < nTotalSockets; index++) 
		{
			SOCKETINFO* ptr = SocketInfoArray[index];

			if (FD_ISSET(ptr->sock, &rset))
			{
				int result = Packing_Recv(ptr);

				switch (result) {
				case DISCONNECTED:
					ptr->state = DISCONNECTED_STATE;
					break;

				case SOCKET_FALSE:
					continue;

				case SOCKET_TRUE:
					break;
				}

				Recv_PackingProcess(ptr);
			}

			if (FD_ISSET(ptr->sock, &wset))
			{
				int result;

				if (ptr->sendbytes == 0) Send_PackingProcess(ptr);
				result = Result_Send(ptr);

				if (result == SOCKET_FALSE) continue;
			}

			if (ptr->state == DISCONNECTED_STATE) 
			{
				RemoveSocketInfo(ptr, index);
				continue;
			}
		}
	}

	// 윈속 종료
	WSACleanup();
	return 0;
}

SOCKET ListenSocket_Init() 
{
	int retval;
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 넌블로킹 소켓으로 전환
	u_long on = 1;
	retval = ioctlsocket(listen_sock, FIONBIO, &on);
	if (retval == SOCKET_ERROR) err_display("ioctlsocket()");

	return listen_sock;
}

// 소켓 정보 추가
BOOL AddSocketInfo(SOCKET sock, SOCKADDR_IN addr)
{
	if (nTotalSockets >= FD_SETSIZE)
	{
		printf("[오류] 소켓 정보를 추가할 수 없습니다!\n");
		return FALSE;
	}

	SOCKETINFO* ptr = new SOCKETINFO;
	if (ptr == NULL) 
	{
		printf("[오류] 메모리가 부족합니다!\n");
		return FALSE;
	}

	ptr->sock = sock;
	memcpy(&ptr->addr, &addr, sizeof(SOCKADDR_IN));

	ptr->total_recvbytes = 0;
	ptr->total_sendbytes = 0;
	ptr->recvbytes = 0;
	ptr->sendbytes = 0;

	ZeroMemory(&ptr->user, sizeof(User_Info));
	ptr->state = INTRO_STATE;

	ptr->flag = false;

	SocketInfoArray[nTotalSockets++] = ptr;

	return TRUE;
}

// 소켓 정보 삭제
void RemoveSocketInfo(SOCKETINFO* ptr, int& index)
{
	// 클라이언트 정보 얻기
	printf("\n[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));


	for (int i = 0; i < nTotalSockets; i++) 
	{
		if (SocketInfoArray[i]->sock == ptr->sock) 
		{
			closesocket(ptr->sock);
			delete SocketInfoArray[i];

			for (int j = i; j < nTotalSockets - 1; j++)
			{
				SocketInfoArray[j] = SocketInfoArray[j + 1];
			}

			SocketInfoArray[nTotalSockets - 1] = nullptr;
			nTotalSockets -= 1;
			index -= 1;
			break;
		}
	}
}

// 소켓 함수 오류 출력 후 종료
void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}