#pragma once
#include "Global.h"

void err_quit(const char* msg);
void err_display(const char* msg);

int recvn(SOCKET s, char* buf, int len, int flags);
bool UnPackingRecv(SOCKET s, char* buf);

int memcpy_send(void* ptr, const void* dst, size_t size, int* len);
int memcpy_recv(void* dst, const void* ptr, size_t size);

PROTOCOL GetProtocol(const char* buf);

int Packing(char* buf, const PROTOCOL protocol);
int Packing(char* buf, const PROTOCOL protocol, const char* str1);
int Packing(char* buf, const PROTOCOL protocol, const char* str1, const char* str2);

void UnPacking(const char* buf, char* str);
void UnPacking(const char* buf, RESULT& result, char* str);
void UnPacking(const char* buf, char* str, char* str2);
