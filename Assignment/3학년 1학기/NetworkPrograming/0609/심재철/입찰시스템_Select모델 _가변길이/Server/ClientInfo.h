#pragma once
#include "Global.h"
#include "AuctionInfo.h"

class _Try_AuctionInfo
{
private:
	_AuctionInfo* info;
	int try_price;
	int result;
public:
	_Try_AuctionInfo()
	{
		info = nullptr;
		result = NODATA;
		try_price = 0;
	}

	~_Try_AuctionInfo()
	{

	}

	void Set_AuctionInfo(_AuctionInfo* _info) { info = _info; }
	void Set_TryPrice(const int price) { try_price = price; }
	void Set_Result(const int _result) { result = _result; }

	_AuctionInfo* Get_AuctionInfo() { return info; }
	const int Get_TryPrice() { return try_price; }
	const int Get_Result() { return result; }
};

class _ClientInfo
{
private:
	SOCKET sock;
	SOCKADDR_IN addr;

	_User_Info* user;
	_User_Info* temp_user;

	STATE pre_state;
	STATE state;

	bool loginstate;

	_Try_AuctionInfo* try_auction;

	int recvbyte;
	int comp_recvbyte;
	bool recv_sizeflag;

	int sendbyte;
	int comp_sendbyte;

	char recv_buf[BUFSIZE];
	char send_buf[BUFSIZE];

public:
	_ClientInfo();
	~_ClientInfo() 
	{
		if (temp_user != nullptr) delete temp_user;
		if (try_auction != nullptr) delete try_auction;
	}

	void Set_Socket(const SOCKET _sock) { sock = _sock; }
	void Set_SockAddr(const SOCKADDR_IN _addr) { memcpy(&addr, &_addr, sizeof(SOCKADDR_IN)); }
	void Set_UserInfo() { user = temp_user; }
	void Set_UserInfo(_User_Info* _user) { user = _user; }
	void Set_PreState(STATE _state) { pre_state = _state; }
	void Set_State(STATE _state) { state = _state; }
	void Set_TryAuction(_Try_AuctionInfo* _try_auction) { try_auction = _try_auction; }
	void Set_RecvByte(const int byte) { recvbyte = byte; }
	void Set_Comp_RecvByte(const int byte) { comp_recvbyte = byte; }
	void Set_SendByte(const int byte) { sendbyte = byte; }
	void Set_Comp_SendByte(const int byte) { comp_sendbyte = byte; }

	const SOCKET Get_Socket() { return sock; }
	const SOCKADDR_IN Get_SockAddr() { return addr; }
	_User_Info* Get_UserInfo() { return user; }
	_User_Info* Get_Temp_UserInfo() { return temp_user; }
	const STATE Get_PreState() { return pre_state; }
	const STATE Get_State() { return state; }
	_Try_AuctionInfo* Get_TryAuction() { return try_auction; }
	const int Get_RecvByte() { return recvbyte; }
	const int Get_Comp_RecvByte() { return comp_recvbyte; }
	const int Get_SendByte() { return sendbyte; }
	const int Get_Comp_SendByte() { return comp_sendbyte; }

	char* Set_RecvBuffer() { return recv_buf; }
	char* Set_SendBuffer() { return send_buf; }
	const char* Get_RecvBuffer() { return recv_buf; }
	const char* Get_SendBuffer() { return send_buf; }


	const bool Get_LoginState() { return loginstate; }
	const bool Get_Recv_Sizeflag() { return recv_sizeflag; }

	void Recv_Sizeflag_Complete() { recv_sizeflag = true; }
	void Recv_Sizeflag_NotComplete() { recv_sizeflag = false; }

	void login(_User_Info* _info);
	void logout();
	void auction_start(_AuctionInfo* _info);
	void auction_end();
	void try_auction_success();
	bool check_auction_success();
	bool check_money(const int _price);
	void deposit(const int _price);
	bool check_send_complete();
	bool is_disconnected();
};