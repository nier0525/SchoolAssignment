#pragma once

#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 9050
#define BUFSIZE    4092
#define MSGSIZE 128
#define USERSIZE 100
#define MAXSIZE 10

#define ERROR_DISCONNECTED -2
#define DISCONNECTED -1
#define SOCKET_TRUE 1
#define SOCKET_FALSE 0

#define INTRO_MSG "\n1. ȸ������\n2. �α���\n3. ����\n\n�Է� : "
#define LOGIN_INTRO_MSG "\n1. ��� ����\n2. ��� ��ǰ\n3. �Ա� ��û\n4. �ܾ� Ȯ��\n5. �α׾ƿ�\n6. ȸ��Ż��\n\n�Է� : "
#define AUCTION_INTRO_MSG "\n[ ���� �� ��� ��ǰ�� �����ϼ���. ]\n\n"
#define AUCTION_ERROR_MSG "\n�����ϰ� �ִ� ���� �����ϰų�, ��� ��ǰ�� ���� ���ݺ��� ���� ������ �Է��߽��ϴ�.\n"
#define AUCTION_WAIT_MSG "\n��ſ� �����Ͽ����ϴ�. ��� �� ����� ���ɴϴ� . . .\n"

#define AUCTION_SUCCESS_MSG "\n�����մϴ�. ��� ������ �����߽��ϴ�.\n"
#define AUCTION_FAIL_MSG "\n��� ������ �����߽��ϴ�.\n"

#define NONE_AUCTION_MSG "\n[ ��Ű� ����Ǿ����ϴ� ]\n"
#define NONE_WARE_MSG "\n���� ���� ��� ��ǰ�� �����ϴ�.\n"

#define ID_MSG "ID : "
#define PW_MSG "PW : "
#define DEPOSIT_MSG "�ݾ� : "

#define NEW_MSG "\n���������� ȸ������ �Ǿ����ϴ�.\n"
#define NEW_ERROR_MSG "\n�ߺ��� ���̵��Դϴ�.\n"
#define LOGIN_MSG "\n���������� �α��� �Ǿ����ϴ�.\n"
#define ID_ERROR_MSG "\n���� ID �Դϴ�.\n"
#define PW_ERROR_MSG "\nPW �� Ʋ�Ƚ��ϴ�.\n"
#define OVERRAP_ERROR_MSG "\n���� ���� �����Դϴ�.\n"

#define DISPOSIT_SUCCESS_MSG "\n���������� �Ա� �Ǿ����ϴ�.\n"
#define LOGOUT_MSG "\n���������� �α׾ƿ� �Ǿ����ϴ�.\n"
#define REMOVE_MSG "\nȸ��Ż�� �Ǿ����ϴ�.\n"

// ��������
enum PROTOCOL
{
	INTRO = 1,
	INTRO_SELECT_MENU,
	LOGIN_SELECT_MENU,
	AUCTION_SELECT,

	ID_PW_MSG,
	NEW_INFO,
	LOGIN_INFO,
	
	DEPOSIT,
	DEPOSIT_NEW,
	DEPOSIT_LOGIN,
	DEPOSIT_SUCCESS,

	AUCTION_END,
	AUCTIONLIST_VIEW,
	AUCTION_DEPOSIT_INFO,
	AUCTION_DEPOSIT_RESULT,

	AUCTION_ERROR,
	AUCTION_WAIT,
	AUCTION_RESULT,

	WARE_VIEW,
	MONEY_VIEW,

	NEW_RESULT,
	LOGIN_RESULT,

	LOGIN_INTRO,
	LOGOUT,
	REMOVE
};

// ȸ������, �α����� �����ϱ� ���� �ι�° ��������
enum SECOND_PROTOCOL
{
	SECOND_NEW = 1,
	SECOND_LOGIN
};

// ����
enum STATE
{
	INTRO_STATE = 1,
	MENU_SELECT_STATE,
	
	INFO_STATE,
	NEW_INFO_STATE,
	USER_INFO_STATE,

	DEPOSIT_STATE,
	DEPOSIT_INFO_STATE,
	DEPOSIT_RESULT_STATE,
	
	NEW_STATE,
	LOGIN_STATE,
	
	AUCTION_INTRO_STATE,
	AUCTION_DEPOSIT_STATE,
	AUCTION_DEPOSIT_RESULT_STATE,
	AUCTION_ERROR_STATE,
	AUCTION_WAIT_MSG_STATE,
	AUCTION_WAIT_STATE,

	AUCTION_FAIL_STATE,
	AUCTION_SUCCESS_STATE,

	WARE_STATE,
	MONEY_STATE,

	LOGIN_INTRO_STATE,
	LOGIN_SELECT_STATE,

	LOGOUT_STATE,
	REMOVE_STATE,
	DISCONNECTED_STATE
};

// �α��� �����
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

// �ʱ� �޴�
enum INTRO_MENU
{
	NEW_MENU = 1,
	LOGIN_MENU,
	EXIT_MENU
};

// �α��� �� �޴�
enum LOGIN_MENU
{
	AUCTION_MENU = 1,
	WARE_MENU,
	DEPOSIT_MENU,
	MONEY_MENU,
	LOGOUT_MENU,
	REMOVE_MENU,
};

// ��� ��ǰ ���� ����ü
struct Ware_Info
{
	char name[MSGSIZE];
	int min_money;

	Ware_Info()
	{
		ZeroMemory(name, sizeof(name));
		min_money = 0;
	}

	Ware_Info(const char* _name, const int _money)
	{
		strcpy(name, _name);
		min_money = _money;
	}
};

// ���� ���� ����ü
struct User_Info
{
	char id[MSGSIZE];
	char pw[MSGSIZE];

	// ���� ������ ���� ������ üũ
	// ���� ���̸� �ٸ� ������ ���� �������� ���� �� �� ���Բ� üŷ
	bool state = false;

	// ���� ���� �� ( �¶��� ���� ��� )
	int money;
	// ��ſ� ���� �� ��
	int deposit_money;

	// ��� ��ǰ ����Ʈ ( ��ſ��� ���� ���� �� ������ �߰� )
	Ware_Info warelist[MAXSIZE];
	int count;
};

// Ŭ���̾�Ʈ ���� ����ü
struct Client_Info
{
	// ���ϰ� �ּ� ����
	SOCKET sock;
	SOCKADDR_IN addr;

	// ���� ���ۿ� �۽� ����
	char recv_buf[BUFSIZE + 1];
	char send_buf[BUFSIZE + 1];

	// ����,�۽� �� ������ ���� ũ��
	int total_recvbytes;
	int total_sendbytes;

	// ����,�۽� �� �������� ũ��
	int recvbytes;
	int sendbytes;

	// ���� ����
	User_Info user;

	// ����
	STATE state;

	// �ٿ뵵�� ���� char �� ������ ���� ���� ��Ŷ�� ���� �� ���� bool ����
	char msg[MSGSIZE];
	bool recv_flag;
};

// ��� ���� ����ü
struct Auction_Info
{
	// ��� ��ǰ
	Ware_Info ware;

	// ���� �� Ŭ���̾�Ʈ ����Ʈ
	Client_Info* client[MAXSIZE];
	// Ŭ���̾�Ʈ �ִ� ���� �ο�
	int max_Count;
	// ���� ���� ���� Ŭ���̾�Ʈ �ο�
	int count;

	Auction_Info()
	{
		ware = Ware_Info();
		max_Count = 0;
		count = 0;
	}

	Auction_Info(const char* name, const int money, const int max, const int _count)
	{		
		ware = Ware_Info(name, money);
		max_Count = max;
		count = _count;
	}
};
