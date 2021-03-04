#pragma once
#include "Global.h"

class _SocketInfo
{
private:
	SOCKET server_sock;
	SOCKET dummy_sock;
	SOCKET client_sock;

	SOCKADDR_IN server_addr;
	SOCKADDR_IN dummy_addr;
	SOCKADDR_IN client_addr;

	HANDLE hEvent;
public:
	_SocketInfo() { ZeroMemory(&*this, sizeof(*this)); }
	~_SocketInfo() {}

	void Set_ServerSock(SOCKET sock) { server_sock = sock; }
	void Set_DummySock(SOCKET sock) { dummy_sock = sock; }
	void Set_ClientSock(SOCKET sock) { client_sock = sock; }

	void Set_ServerAddr(SOCKADDR_IN addr) { memcpy(&server_addr, &addr, sizeof(addr)); }
	void Set_DummyAddr(SOCKADDR_IN addr) { memcpy(&dummy_addr, &addr, sizeof(addr)); }
	void Set_ClientAddr(SOCKADDR_IN addr) { memcpy(&client_addr, &addr, sizeof(addr)); }

	void Set_Event(HANDLE _event) { hEvent = _event; }
	void Set_Event() { SetEvent(hEvent); }

	SOCKET Get_ServerSock() { return server_sock; }
	SOCKET Get_DummySock() { return dummy_sock; }
	SOCKET Get_ClientSock() { return client_sock; }

	SOCKADDR_IN Get_ServerAddr() { return server_addr; }
	SOCKADDR_IN Get_DummyAddr() { return dummy_addr; }
	SOCKADDR_IN Get_ClientAddr() { return client_addr; }

	HANDLE Get_Event() { return hEvent; }
};