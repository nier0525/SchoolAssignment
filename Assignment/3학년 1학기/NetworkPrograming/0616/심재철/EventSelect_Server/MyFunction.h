#pragma once
#include "Global.h"

void err_quit(const char* msg);
void err_display(const char* msg);
void err_display(const int errcode);

PROTOCOL GetProtocol(const char* buf);

int memcpy_send(void* ptr, const void* dst, size_t size, int* len);
int memcpy_recv(void* dst, const void* ptr, size_t size);

int Message_Recv(ClientInfo* ptr);
int Packing_Recv(ClientInfo* ptr);

// Packing
int Packing(char* buf, const PROTOCOL protocol, const char* str1);
int Packing(char* buf, const PROTOCOL protocol, const RESULT result, const char* str1);
int Packing(char* buf, const PROTOCOL protocol, const char* str1, const char* str2);

// UnPacking
void UnPacking(const char* buf, char* str);
void UnPacking(const char* buf, char* str, char* str2);