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
#define SOC_ERROR 1
#define SOC_TRUE 2
#define SOC_FALSE 3

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
	JOIN = 1,
	LOGIN,
	EXIT,
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
	MENU_SELECT_STATE = 1,
	RESULT_SEND_STATE,
	JOIN_STATE,
	LOGIN_STATE,

	DISCONNECTED_STATE
};

typedef struct _User_Info
{
	char id[MSGSIZE];
	char pw[MSGSIZE];

	bool state;
}UserInfo, *LPUserInfo;

enum IO_TYPE
{
	IO_RECV = 1,
	IO_SEND
};

struct _Client_Info;

typedef struct _WSAOVERLAPPED_EX
{
	WSAOVERLAPPED overlapped;
	_Client_Info* ptr;
	IO_TYPE type;
}WSAOVERLAPPED_EX, *LPWSAOVERLAPPED_EX;

typedef struct _Client_Info
{
	WSAOVERLAPPED_EX r_overlapped;
	WSAOVERLAPPED_EX s_overlapped;

	SOCKET sock;
	SOCKADDR_IN addr;

	UserInfo user;
	STATE state;
	bool r_sizeflag;

	int recvbytes;
	int comp_recvbytes;
	int sendbytes;
	int comp_sendbytes;

	char recv_buf[BUFSIZE];
	char send_buf[BUFSIZE];

	WSABUF r_wsabuf;
	WSABUF s_wsabuf;

}ClientInfo, *LPClientInfo;
