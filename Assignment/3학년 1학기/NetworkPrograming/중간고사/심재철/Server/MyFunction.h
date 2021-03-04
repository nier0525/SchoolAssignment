#pragma once

#include "Global.h"

// ���� �ʱ�ȭ �Լ�
void ListenSocket_Init(SOCKET& sock, SOCKADDR_IN& addr);

// ���� �Լ� ���� ��� �� ����
void err_quit(const char* msg);

// ���� �Լ� ���� ���
void err_display(const char* msg);

// �������� ����
PROTOCOL GetProtocol(char* buf);

// ���� ���� ��ŷ �Լ�
int memcpy_send(void* ptr, const void* dst, size_t size, int* len);

// ���� ���� ����ŷ �Լ�
int memcpy_recv(void* dst, const void* ptr, size_t size);

// �� ���ŷ ���� �Լ�
int Message_Recv(Client_Info* ptr);

// �� ���ŷ �۽� �Լ�
int Message_Send(Client_Info* ptr);

// ���� ���� ����
int Packing_Recv(Client_Info* ptr);

// �۽� ���� üũ
int Result_Send(Client_Info* ptr);


// Packing
int Packing(char* buf, PROTOCOL protocol, const char* str1);

int Packing(char* buf, PROTOCOL protocol, SECOND_PROTOCOL second_protocol, const char* str1);

int Packing(char* buf, PROTOCOL protocol, RESULT result, const char* str1);

int Packing(char* buf, PROTOCOL protocol, const char* str1, const char* str2);

int Packing(char* buf, PROTOCOL protocol, SECOND_PROTOCOL second_protocol, const char* str1, const char* str2);

// UnPacking
void UnPacking(const char* buf, char* str);

void UnPacking(const char* buf, char* str, char* str2);