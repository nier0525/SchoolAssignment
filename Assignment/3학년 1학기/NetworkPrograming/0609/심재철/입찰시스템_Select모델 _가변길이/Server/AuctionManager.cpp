#include "AuctionManager.h"
IMPLEMENT_SINGLETON(_AuctionManager)

_AuctionManager::_AuctionManager()
{
	Auction_List = new CLinkedList<_AuctionInfo*>();
	auction_user_count = 0;
}

_AuctionInfo* _AuctionManager::AddAuction(const char* product, int count, int price, CRITICAL_SECTION cs)
{
	EnterCriticalSection(&cs);
	_AuctionInfo* info = new _AuctionInfo;

	info->Set_Product(product);
	info->Set_UserCount(count);
	info->Set_Price(price);
	info->Set_Code(++auction_user_count);
	info->Set_State(AUCTION_ONGOING);
	Auction_List->Insert(info);

	DeleteCriticalSection(&cs);
	return info;
}

void _AuctionManager::RemoveAuction(_AuctionInfo* info, CRITICAL_SECTION cs)
{
	EnterCriticalSection(&cs);
	Auction_List->Delete(info);
	delete info;
	DeleteCriticalSection(&cs);
}

_AuctionInfo* _AuctionManager::SearchAuction(int code)
{
	_AuctionInfo* info = nullptr;
	Auction_List->SearchStart();

	while (1)
	{
		info = Auction_List->SearchData();
		if (info == nullptr) break;
		if (info->Get_State() == AUCTION_COMPLETE) continue;
		if (info->Get_Code() == code) break;
	}

	Auction_List->SearchEnd();
	return info;
}

bool _AuctionManager::AllAuctionComplete()
{
	Auction_List->SearchStart();

	while (1)
	{
		_AuctionInfo* info = Auction_List->SearchData();
		if (info == nullptr) break;
		if (info->Get_State() != AUCTION_COMPLETE) return false;
	}
	return true;
}