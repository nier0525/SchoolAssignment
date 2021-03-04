#pragma once
//#define _CRT_SECURE_NO_WARNINGS

#include <WinSock2.h>
#pragma comment(lib, "ws2_32")

#include <stdio.h>
#include <stdlib.h>

#define SERVERPORT 9000
#define BUFSIZE 4096
#define MSGSIZE 128

#define ERROR_DISCONNECTED -2
#define DISCONNECTED -1
#define SOCKET_TRUE 1
#define SOCKET_FALSE 0

#define INTRO_MENU_MSG "\n1. 회원가입\n2. 로그인\n3. 종료\n\n입력 : "
#define ID_MSG "ID : "
#define PW_MSG "PW : "
#define ERROR_JOIN_EXISTS_MSG "\n중복된 아이디 입니다.\n"
#define JOIN_SUCCESS_MSG "\n회원가입에 성공했습니다.\n"
#define ERROR_LOGIN_ID_MSG "\n해당 ID 는 없는 ID 입니다.\n"
#define ERROR_LOGIN_PW_MSG "\nPW 가 틀렸습니다.\n"
#define ERROR_LOGIN_EXISTS_MSG "\n현재 접속중인 계정입니다.\n"
#define LOGIN_SUCCESS_MSG "\n로그인에 성공했습니다.\n"
#define LOGIN_MENU_MSG "\n1. 로그아웃\n2. 회원탈퇴\n\n입력 : "
#define LOGOUT_MSG "\n로그아웃 하셨습니다.\n"
#define REMOVE_MSG "\n회원탈퇴 하셨습니다.\n"

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

enum STATE
{
	INTRO_STATE = 1,
	INTRO_MENU_SELECT_STATE,
	JOIN_RESULT_STATE,
	LOGIN_RESULT_STATE,
	LOGIN_STATE,
	LOGIN_MENU_SELECT_STATE,
	DISCONNECTED_STATE
};

struct UserInfo
{
	char id[MSGSIZE];
	char pw[MSGSIZE];

	bool login_state;
};

struct ClientInfo
{
	SOCKET sock;
	SOCKADDR_IN addr;

	char recv_buf[BUFSIZE + 1];
	char send_buf[BUFSIZE + 1];

	int recv_bytes;
	int comp_recv_bytes;
	int send_bytes;
	int comp_send_bytes;

	bool sizeflag;
	bool recvflag;

	UserInfo user;
	UserInfo temp_user;

	STATE state;
};
