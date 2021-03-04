#pragma once

#include "Global.h"

void err_quit(const char* msg);
void err_display(const char* msg);
void err_display(int errcode);

bool Recv(ClientInfo* _ptr);
int CompleteRecv(ClientInfo* ptr, int _completebyte);

bool Send(ClientInfo* _ptr, int _size);
int CompleteSend(ClientInfo* _ptr, int _completebyte);

PROTOCOL GetProtocol(char* buf);

void PackPacket(char* buf, PROTOCOL protocol, int& size);
void PackPacket(char* buf, PROTOCOL protocol, char map[HEIGHT][WIDTH], char map2[HEIGHT][WIDTH], int stage, int stage2, int x, int y, int x1, int y1, int& size);

void UnPackPacket(char* buf, int& x, int& y);
void UnPackPacket(char* buf, int& x, int& y, int& x1, int& y1);