#pragma once

#pragma comment(lib, "ws2_32")

#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9050
#define BUFSIZE 4092
#define MSGSIZE 128

#define MAXSIZE 5

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

// ȸ������, �α��� �и��� ���� �ι�° ��������
enum SECOND_PROTOCOL
{
	SECOND_NEW = 1,
	SECOND_LOGIN
};

// �α��� ���
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

// �޴� ��
enum INTRO_MENU
{
	NEW_MENU = 1,
	LOGIN_MENU,
	EXIT_MENU
};

// ��� ��ǰ ���� ����ü
struct Ware_Info
{
	char name[MSGSIZE];
	int min_money;
};

// ���� ���� ����ü
struct User_Info
{
	char id[MSGSIZE];
	char pw[MSGSIZE];

	User_Info()
	{
		ZeroMemory(id, sizeof(MSGSIZE));
		ZeroMemory(pw, sizeof(MSGSIZE));
	}
};