#pragma once
#pragma warning(disable : 4996)

#include <WinSock2.h>
#pragma comment(lib, "ws2_32")

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 포트 번호 , 버퍼 사이즈
#define PORT 9050
#define BUFSIZE 4096

// 스테이지 총 개수, 맵 가로 세로 크기
#define MAXSTAGE 3
#define WIDTH 21
#define HEIGHT 18

// 프로토콜
enum PROTOCOL
{
	JOIN = 1,
	MATCH,
	SETTING,
	MAPSETTING,
	RESET,
	UPDATE,
	MOVE,

	REMATCH,

	WIN,
	LOSE,
	EXIT,
};

// 상태
enum STATE
{
	GAME_STATE = 1,
	SEND_RESULT_STATE,
	DISCONNECTED_STATE
};

// 송수신 처리 결과
enum
{
	SOC_ERROR = 1,
	SOC_TRUE,
	SOC_FALSE
};

// Overlapped Type
enum IO_TYPE
{
	IO_RECV = 1,
	IO_SEND
};

struct ClientInfo;

// OverLapped 사용자 정의 구조체
struct WSAOverLapped_EX
{
	WSAOVERLAPPED overlapped;
	ClientInfo* ptr;
	IO_TYPE type;
};

// 유저 게임 정보
struct UserInfo
{
	int Stage;										// 현재 게임 스테이지
	char Map[HEIGHT][WIDTH];		// 맵

	int x;												// 플레이어 위치 값
	int y;
};

// 클라언트 정보 구조체
struct ClientInfo
{
	// 송수신 Overlapped
	WSAOverLapped_EX r_overlapped;
	WSAOverLapped_EX s_overlapped;

	// 소켓, 주소, 상태
	SOCKET sock;
	SOCKADDR_IN addr;
	STATE state;
	bool r_sizeflag;

	// 송수신 바이트 및 완료된 바이트
	int recvbytes;
	int comp_recvbytes;
	int sendbytes;
	int comp_sendbytes;

	// 송수신 버퍼
	char recvbuf[BUFSIZE];
	char sendbuf[BUFSIZE];

	// 송수신 WSA 버퍼
	WSABUF r_wsabuf;
	WSABUF s_wsabuf;

	// 유저 게임 정보
	UserInfo user;
	// 게임 상대
	ClientInfo* part;
};