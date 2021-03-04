#pragma once
#pragma warning(disable : 4996)

#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9050
#define BUFSIZE 4096
#define MSGSIZE 128

enum PROTOCOL
{
	JOIN = 1,
	LOGIN,
	EXIT,
	JOIN_INFO,
	JOIN_RESULT,
	LOGIN_INFO,
	LOGIN_RESULT,
};

enum STATE
{
	MENU_SELECT_STATE = 1,
	RESULT_STATE,
};

enum RESULT
{
	NODATA = -1,
	JOIN_SUCCESS = 1,
	ERROR_JOIN_EXISTS,
	LOGIN_SUCCESS,
	ERROR_LOGIN_ID,
	ERROR_LOGIN_PW,
	ERROR_LOGIN_EXISTS
};

struct UserInfo
{
	char id[MSGSIZE];
	char pw[MSGSIZE];
};
