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

#define INTRO_MSG "\n1. 회원가입\n2. 로그인\n3. 종료\n\n입력 : "
#define LOGIN_INTRO_MSG "\n1. 경매 참여\n2. 경매 물품\n3. 입금 신청\n4. 잔액 확인\n5. 로그아웃\n6. 회원탈퇴\n\n입력 : "
#define AUCTION_INTRO_MSG "\n[ 참여 할 경매 물품을 선택하세요. ]\n\n"
#define AUCTION_ERROR_MSG "\n소지하고 있는 돈이 부족하거나, 경매 물품의 최저 가격보다 적은 가격을 입력했습니다.\n"
#define AUCTION_WAIT_MSG "\n경매에 참가하였습니다. 잠시 후 결과가 나옵니다 . . .\n"

#define AUCTION_SUCCESS_MSG "\n축하합니다. 경매 낙찰을 성공했습니다.\n"
#define AUCTION_FAIL_MSG "\n경매 낙찰에 실패했습니다.\n"

#define NONE_AUCTION_MSG "\n[ 경매가 종료되었습니다 ]\n"
#define NONE_WARE_MSG "\n낙찰 받은 경매 물품이 없습니다.\n"

#define ID_MSG "ID : "
#define PW_MSG "PW : "
#define DEPOSIT_MSG "금액 : "

#define NEW_MSG "\n정상적으로 회원가입 되었습니다.\n"
#define NEW_ERROR_MSG "\n중복된 아이디입니다.\n"
#define LOGIN_MSG "\n정상적으로 로그인 되었습니다.\n"
#define ID_ERROR_MSG "\n없는 ID 입니다.\n"
#define PW_ERROR_MSG "\nPW 가 틀렸습니다.\n"
#define OVERRAP_ERROR_MSG "\n접속 중인 계정입니다.\n"

#define DISPOSIT_SUCCESS_MSG "\n정상적으로 입금 되었습니다.\n"
#define LOGOUT_MSG "\n정상적으로 로그아웃 되었습니다.\n"
#define REMOVE_MSG "\n회원탈퇴 되었습니다.\n"

// 프로토콜
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

// 회원가입, 로그인을 구분하기 위한 두번째 프로토콜
enum SECOND_PROTOCOL
{
	SECOND_NEW = 1,
	SECOND_LOGIN
};

// 상태
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

// 로그인 결과값
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

// 초기 메뉴
enum INTRO_MENU
{
	NEW_MENU = 1,
	LOGIN_MENU,
	EXIT_MENU
};

// 로그인 후 메뉴
enum LOGIN_MENU
{
	AUCTION_MENU = 1,
	WARE_MENU,
	DEPOSIT_MENU,
	MONEY_MENU,
	LOGOUT_MENU,
	REMOVE_MENU,
};

// 경매 물품 정보 구조체
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

// 유저 정보 구조체
struct User_Info
{
	char id[MSGSIZE];
	char pw[MSGSIZE];

	// 현재 유저가 접속 중인지 체크
	// 접속 중이면 다른 유저가 같은 계정으로 접속 할 수 없게끔 체킹
	bool state = false;

	// 현재 가진 돈 ( 온라인 통장 기능 )
	int money;
	// 경매에 입찰 할 돈
	int deposit_money;

	// 경매 물품 리스트 ( 경매에서 낙찰 성공 할 때마다 추가 )
	Ware_Info warelist[MAXSIZE];
	int count;
};

// 클라이언트 정보 구조체
struct Client_Info
{
	// 소켓과 주소 정보
	SOCKET sock;
	SOCKADDR_IN addr;

	// 수신 버퍼와 송신 버퍼
	char recv_buf[BUFSIZE + 1];
	char send_buf[BUFSIZE + 1];

	// 수신,송신 한 데이터 현재 크기
	int total_recvbytes;
	int total_sendbytes;

	// 수신,송신 할 데이터의 크기
	int recvbytes;
	int sendbytes;

	// 유저 정보
	User_Info user;

	// 상태
	STATE state;

	// 다용도로 쓰일 char 형 변수와 가변 길이 패킷을 받을 때 쓰일 bool 변수
	char msg[MSGSIZE];
	bool recv_flag;
};

// 경매 정보 구조체
struct Auction_Info
{
	// 경매 물품
	Ware_Info ware;

	// 참가 한 클라이언트 리스트
	Client_Info* client[MAXSIZE];
	// 클라이언트 최대 참가 인원
	int max_Count;
	// 현재 참가 중인 클라이언트 인원
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
