#pragma once
#pragma warning(disable : 4996)

#include <WinSock2.h>
#pragma comment(lib, "ws2_32")

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ��Ʈ ��ȣ , ���� ������
#define PORT 9050
#define BUFSIZE 4096

// �������� �� ����, �� ���� ���� ũ��
#define MAXSTAGE 3
#define WIDTH 21
#define HEIGHT 18

// ��������
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

// ����
enum STATE
{
	GAME_STATE = 1,
	SEND_RESULT_STATE,
	DISCONNECTED_STATE
};

// �ۼ��� ó�� ���
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

// OverLapped ����� ���� ����ü
struct WSAOverLapped_EX
{
	WSAOVERLAPPED overlapped;
	ClientInfo* ptr;
	IO_TYPE type;
};

// ���� ���� ����
struct UserInfo
{
	int Stage;										// ���� ���� ��������
	char Map[HEIGHT][WIDTH];		// ��

	int x;												// �÷��̾� ��ġ ��
	int y;
};

// Ŭ���Ʈ ���� ����ü
struct ClientInfo
{
	// �ۼ��� Overlapped
	WSAOverLapped_EX r_overlapped;
	WSAOverLapped_EX s_overlapped;

	// ����, �ּ�, ����
	SOCKET sock;
	SOCKADDR_IN addr;
	STATE state;
	bool r_sizeflag;

	// �ۼ��� ����Ʈ �� �Ϸ�� ����Ʈ
	int recvbytes;
	int comp_recvbytes;
	int sendbytes;
	int comp_sendbytes;

	// �ۼ��� ����
	char recvbuf[BUFSIZE];
	char sendbuf[BUFSIZE];

	// �ۼ��� WSA ����
	WSABUF r_wsabuf;
	WSABUF s_wsabuf;

	// ���� ���� ����
	UserInfo user;
	// ���� ���
	ClientInfo* part;
};