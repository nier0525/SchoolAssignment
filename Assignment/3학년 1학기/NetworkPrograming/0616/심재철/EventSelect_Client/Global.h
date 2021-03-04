#pragma once
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 4096
#define MSGSIZE 128

enum PROTOCOL
{
	INIT = -1,
	INTRO_MENU = 1,
	JOIN,
	LOGIN,
	EXIT,
	ID_PW_INFO,
	JOIN_INFO,
	LOGIN_INFO,
	JOIN_RESULT,
	LOGIN_RESULT,
	LOGIN_MENU,
	LOGOUT,
	REMOVE,
	LOGOUT_RESULT,
	REMOVE_RESULT,
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