#pragma once
#include "Global.h"

// ���� �ʱ�ȭ �Լ�
void Socket_Init(SOCKET& sock, SOCKADDR_IN& addr);

// ���� �Լ� ���� ��� �� ����
void err_quit(const char* msg);

// ���� �Լ� ���� ���
void err_display(const char* msg);

// ����� ���� ������ ���� �Լ�
int recvn(SOCKET s, char* buf, int len, int flags);

// �������� ������ ���� �Լ�
bool UnPackingRecv(SOCKET s, char* buf);

// ���� ���� ��ŷ �Լ�
int memcpy_send(void* ptr, const void* dst, size_t size, int* len);

// ���� ���� ����ŷ �Լ�
int memcpy_recv(void* dst, const void* ptr, size_t size);

// �������� ����
PROTOCOL GetProtocol(char* buf);

// Packing
int Packing(char* buf, PROTOCOL protocol, const char* str1);

int Packing(char* buf, PROTOCOL protocol, const char* str1, const char* str2);

// UnPacking
void UnPacking(const char* buf, char* str);

void UnPacking(const char* buf, SECOND_PROTOCOL& protocol, char* str);

void UnPacking(const char* buf, RESULT& result, char* str);

void UnPacking(const char* buf, char* str, char* str2);

void UnPacking(const char* buf, SECOND_PROTOCOL& protocol, char* str, char* str2);