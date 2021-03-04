#pragma once
#include "Global.h"
#include "ClientInfo.h"

class _AuctionGroupInfo
{
private:
	_AuctionInfo* auction_info;

	CLinkedList<_ClientInfo*>* user_list;
	CStack<_ClientInfo*, MAX_BID_COUNT>* bid_user;
	int now_auction_count;
public:
	_AuctionGroupInfo(_AuctionInfo* info);
	~_AuctionGroupInfo()
	{
		if (user_list != nullptr) delete user_list;
	}

	_AuctionInfo* Get_Auction_Info() { return auction_info; }
	CLinkedList<_ClientInfo*>* Get_UserList() { return user_list; }
	CStack<_ClientInfo*, MAX_BID_COUNT>* Get_BidUser() { return bid_user; }

	bool userlist_insert(_ClientInfo* _info);
	bool userlist_delete(_ClientInfo* _info);
	bool isfull();
	void searchstart();
	bool searchdata(_ClientInfo*& _info);
	void searchend();
	void increase_timer_count();
	void clear_timer_count();
	int get_timer_count();
	bool check_timer_count();
	int get_user_count();
	bool UpdateMaxBidUser(_ClientInfo* _info);
	bool ChangeMaxBidUser();
	_ClientInfo* GetMaxBidUser();
};