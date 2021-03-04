#pragma once
#pragma warning(disable : 4996)

#include <WinSock2.h>
#pragma comment(lib, "ws2_32")

#include <stdio.h>
#include <stdlib.h>

#define IP "127.0.0.1"
#define PORT 9050
#define BUFSIZE 4096

#define HEIGHT 18
#define WIDTH 21

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
	WAIT_STATE = 1,
	GAME_STATE,
	LOSE_STATE,
	WIN_STATE,
};

// ���� Ű�� ����
enum KEYCODE
{
	LEFT = 1,
	RIGHT,
	UP,
	DOWN,
	Q,
	R,
};

// Ŭ���̾�Ʈ ����
struct ClientInfo
{
	// ����, �ּ�
	SOCKET sock;
	SOCKADDR_IN addr;

	// ����, �ۼ��� ����, �������� ������
	STATE state;
	char r_buf[BUFSIZE];
	char s_buf[BUFSIZE];
	int size;

	// Ű�� ����
	KEYCODE key;

	// ���� �̺�Ʈ
	HANDLE hWrite;
};

void err_quit(const char* msg);
void err_display(const char* msg);
void err_display(int errcode);

int recvn(SOCKET s, char* buf, int len, int flags);
bool PacketRecv(SOCKET _sock, char* _buf);

PROTOCOL GetProtocol(char* buf);

void PackPacket(char* buf, PROTOCOL protocol, int& size);
void PackPacket(char* buf, PROTOCOL protocol, int x, int y, int& size);
void PackPacket(char* buf, PROTOCOL protocol, int x, int y, int x1, int y1, int& size);

void UnPackPacket(char* buf, char map[HEIGHT][WIDTH], char map2[HEIGHT][WIDTH], int& stage, int& stage2, int& x, int& y, int& x1, int& y1);



