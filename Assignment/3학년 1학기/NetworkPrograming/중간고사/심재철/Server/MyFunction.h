#pragma once

#include "Global.h"

// 소켓 초기화 함수
void ListenSocket_Init(SOCKET& sock, SOCKADDR_IN& addr);

// 소켓 함수 오류 출력 후 종료
void err_quit(const char* msg);

// 소켓 함수 오류 출력
void err_display(const char* msg);

// 프로토콜 수신
PROTOCOL GetProtocol(char* buf);

// 가변 길이 패킹 함수
int memcpy_send(void* ptr, const void* dst, size_t size, int* len);

// 가변 길이 언패킹 함수
int memcpy_recv(void* dst, const void* ptr, size_t size);

// 논 블로킹 수신 함수
int Message_Recv(Client_Info* ptr);

// 논 블로킹 송신 함수
int Message_Send(Client_Info* ptr);

// 가변 길이 수신
int Packing_Recv(Client_Info* ptr);

// 송신 상태 체크
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