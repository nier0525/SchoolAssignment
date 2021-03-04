#pragma once;
#pragma warning(disable : 4996)

#include <WinSock2.h>
#pragma comment(lib, "ws2_32")

#include <stdio.h>
#include <stdlib.h>

#define PORT 9050
#define BUFSIZE 4096
#define MSGSIZE 128

#define ERROR_DISCONNECTED -2
#define DISCONNECTED -3
#define SOCKET_TRUE 1
#define SOCKET_FALSE 0

#define INTRO_MSG "\n1. 회원가입\n2. 로그인\n3. 종료\n\n입력 : "
#define ID_MSG "\nID : "
#define PW_MSG "\nPW : "
#define JOIN_SUCCESS_MSG "\n회원가입에 성공했습니다.\n"
#define ERROR_JOIN_MSG "\n이미 있는 ID 입니다.\n"
#define LOGIN_SUCCESS_MSG "\n로그인에 성공했습니다.\n"
#define ERROR_LOGIN_ID_MSG "\n해당 ID 는 없는 ID 입니다.\n"
#define ERROR_LOGIN_PW_MSG "\nPW 가 일치하지 않습니다.\n"
#define ERROR_LOGIN_EXISTS_MSG "\n다른 호스트에서 접속중인 계정입니다.\n"

enum PROTOCOL
{
	INIT = -1,
	INTRO = 1,
	JOIN,
	LOGIN,
	EXIT,
	ID_PW_INFO,
	JOIN_INFO,
	JOIN_RESULT,
	LOGIN_INFO,
	LOGIN_RESULT,
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

	DISCONNECTED_STATE
};

struct _UserInfo
{
	char id[MSGSIZE];
	char pw[MSGSIZE];

	bool state;
};

struct _ClientInfo
{
	WSAOVERLAPPED overlapped;

	SOCKET sock;
	SOCKADDR_IN addr;

	DWORD flag;
	DWORD recvbytes;
	DWORD sendbytes;


	char recv_buf[BUFSIZE];
	char send_buf[BUFSIZE];

	int recv_bytes;
	int comp_recvbytes;
	int send_bytes;
	int comp_sendbytes;

	DWORD comp_bytes;

	bool sizeflag;

	bool recvflag;

	_UserInfo user;
	_UserInfo temp_user;

	STATE state;

	WSABUF wsabuf;
};