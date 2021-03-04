#pragma once
#include "AuctionGroupInfo.h"

class _AuctionGruopManager
{
	DECLARE_SINGLETONE(_AuctionGruopManager)
private:
	CLinkedList<_AuctionGroupInfo*>* Auction_Group_List;

	_AuctionGruopManager();
	~_AuctionGruopManager() 
	{
		if (Auction_Group_List != nullptr) delete Auction_Group_List;
	}
public:
	_AuctionGroupInfo* AddAuctionGroup(_AuctionInfo* info, CRITICAL_SECTION cs);
	void RemoveAuctionGroup(_AuctionGroupInfo* info, CRITICAL_SECTION cs);
	_AuctionGroupInfo* SearchAuctionGroup(_AuctionInfo* info);

	bool CheckAuctionCompleteGroup(_ClientInfo* info);
	void ExitAuctionGroup(_ClientInfo* info);
	bool TryBid(_ClientInfo* info, int price);

	CLinkedList<_AuctionGroupInfo*>* Get_AuctionGroupList() { return Auction_Group_List; }
};