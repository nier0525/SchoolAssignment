#pragma once
#include "Global.h"

// 소켓 초기화 함수
void Socket_Init(SOCKET& sock, SOCKADDR_IN& addr);

// 소켓 함수 오류 출력 후 종료
void err_quit(const char* msg);

// 소켓 함수 오류 출력
void err_display(const char* msg);

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char* buf, int len, int flags);

// 가변길이 데이터 수신 함수
bool UnPackingRecv(SOCKET s, char* buf);

// 가변 길이 패킹 함수
int memcpy_send(void* ptr, const void* dst, size_t size, int* len);

// 가변 길이 언패킹 함수
int memcpy_recv(void* dst, const void* ptr, size_t size);

// 프로토콜 수신
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