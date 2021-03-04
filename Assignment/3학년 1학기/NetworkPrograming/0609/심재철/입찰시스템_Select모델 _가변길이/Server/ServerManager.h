#pragma once
#include "SocketInfo.h"
#include "ClientInfo.h"
#include "AuctionInfo.h"

class _ServerManager
{
	DECLARE_SINGLETONE(_ServerManager)
private:
	_SocketInfo* sock_info;

	_ServerManager();
	~_ServerManager() 
	{
		closesocket(sock_info->Get_ServerSock());
		closesocket(sock_info->Get_DummySock());
		CloseHandle(sock_info->Get_Event());

		if (sock_info != nullptr) delete sock_info;
	}
public:
	void Init();

	void err_quit(const char* msg);
	void err_display(const char* msg);

	void GetProtocol(const char* ptr, PROTOCOL& protocol);

	void PackPacket(char* buf, PROTOCOL protocol, int& size);
	void PackPacket(char* buf, PROTOCOL protocol, const int data, int& size);
	void PackPacket(char* buf, PROTOCOL protocol, const int data, const char* str1, int& size);
	void PackPacket(char* _buf, PROTOCOL _protocol, CLinkedList<_AuctionInfo*>* _list, int& _size);
	void PackPacket(char* _buf, PROTOCOL _protocol, const char* _str1, int& _size);

	void UnPackPacket(const char* _buf, char* _str1, char* _str2, char* _str3, int& _data);
	void UnPackPacket(const char* _buf, char* _str1, char* _str2);
	void UnPackPacket(const char* _buf, int& _data1);
	void UnPackPacket(const char* _buf, int& _data1, int& _data2);
	void UnPackPacket(const char* _buf, _AuctionInfo& _info);

	int MessageRecv(_ClientInfo* _info);
	int MessageSend(_ClientInfo* _info);

	int PacketRecv(_ClientInfo* ptr);
	int SendCheck(_ClientInfo* ptr);

	_SocketInfo* Get_SocketInfo() { return sock_info; }
};