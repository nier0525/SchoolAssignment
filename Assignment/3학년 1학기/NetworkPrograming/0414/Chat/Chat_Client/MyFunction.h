#pragma once

#include "Socket.h"
#include <Windows.h>
#include <math.h>

enum DISPLAYSTATE { LOGIN, ROOM, CHAT };

struct User_Info {
	char name[128];
	char room_ip[128];
	int room_num;
	int room_count;
	char chat[128];
	char msg[128];

	bool state;
};

struct ROOMLIST {
	char roomname[10][ROOMNAME_LEN];
	int roomsize;
	bool memberlist;
};

typedef char* ElementType;

void Add(char Arr[][MSGSIZE], int* count, ElementType data);
void Remove(char Arr[][MSGSIZE], int* count, ElementType data);
void Sort(char Arr[][MSGSIZE], int count);

void InputChar(WPARAM wParam, char* text, int* len, int size);

double Lengthpts(int x1, int y1, int x2, int y2);
bool CollisionEnterPoint(int x, int y, int mx, int my, int x_size, int y_size);

bool RoomView(SocketPakage* socket, ROOMLIST& room);
bool RoomSelect(SocketPakage* sock, User_Info& user, int num);

bool JoinMessage(SocketPakage* sock, User_Info user);
bool SendToMessage(SocketPakage* sock, User_Info& user);
DWORD WINAPI RecvThread(LPVOID arg);

void LoginDisplay(HWND hWnd, HDC hdc, User_Info user);
void RoomSelectDisPlay(HWND hWnd, HDC hdc, User_Info user, ROOMLIST room);
void ChattingDisPlay(HWND hWnd, HDC hdc, User_Info user, ROOMLIST room, int count, char chat[][MSGSIZE]);
void MemberDIsPlay(HWND hWnd, HDC hdc, int count, char list[][MSGSIZE]);