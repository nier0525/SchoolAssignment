#pragma once
#include "AuctionInfo.h"

class _AuctionManager
{
	DECLARE_SINGLETONE(_AuctionManager)
private:
	CLinkedList<_AuctionInfo*>* Auction_List;
	int auction_user_count;

	_AuctionManager();
	~_AuctionManager() { if (Auction_List != nullptr) delete Auction_List; }
public:
	_AuctionInfo* AddAuction(const char* product, int count, int price, CRITICAL_SECTION cs);
	void RemoveAuction(_AuctionInfo* info, CRITICAL_SECTION cs);
	_AuctionInfo* SearchAuction(int code);

	bool AllAuctionComplete();

	CLinkedList<_AuctionInfo*>* Get_AuctionList() { return Auction_List; }
};