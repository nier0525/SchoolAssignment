#pragma once

#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#define IP "127.0.0.1"
#define PORT 9050
#define BUFSIZE 512
#define ROOMNAME_LEN 50

#define MSGSIZE 128
#define MAX_SIZE 19

enum STATE {
	WAIT = 1,
	VIEW,
	SELECT,
	ROOMIP,
	CALL
};

struct SocketPakage;

void err_quit(const char *msg);
void err_display(const char *msg);

void S_SocketInit(SOCKET& sock, SOCKADDR_IN& addr, int _ttl);
void S_AddrChange(SOCKADDR_IN& addr);
void S_AddrChange(SOCKADDR_IN& addr, const char* ip);

void R_SocketInit(SOCKET& sock, SOCKADDR_IN& addr);
bool Multicast_Group_Add(SOCKET& sock, ip_mreq& mreq, const char* ip);
void Multicast_Group_Drop(SOCKET& sock, ip_mreq mreq);

void CleanSocket(SocketPakage* arg);

struct SocketPakage {
	WSADATA wsa;

	SOCKET s_sock;
	SOCKET r_sock;

	SOCKADDR_IN send_addr;
	SOCKADDR_IN recv_addr;
	SOCKADDR_IN peeraddr;

	struct ip_mreq mreq;

	int addrlen;
	int len;

	char buf[BUFSIZE * 2];

	int current;
	char TextArray[MAX_SIZE][MSGSIZE];
	
	int count;
	char UserArray[MAX_SIZE][MSGSIZE];

	SocketPakage() {
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != NULL) err_quit("WSAStartup()");
		S_SocketInit(s_sock, send_addr, 2);
		R_SocketInit(r_sock, recv_addr);

		ZeroMemory(&mreq, sizeof(ip_mreq));
		ZeroMemory(&peeraddr, sizeof(SOCKADDR_IN));
		ZeroMemory(&buf, sizeof(BUFSIZE));
		ZeroMemory(&TextArray, sizeof(TextArray));
		ZeroMemory(&UserArray, sizeof(UserArray));

		addrlen = 0;
		len = 0;
		current = 0;
		count = 0;
	}
};