#include "AuctionGroupManager.h"
IMPLEMENT_SINGLETON(_AuctionGruopManager)

_AuctionGruopManager::_AuctionGruopManager()
{
	Auction_Group_List = new CLinkedList<_AuctionGroupInfo*>();
}

_AuctionGroupInfo* _AuctionGruopManager::AddAuctionGroup(_AuctionInfo* info, CRITICAL_SECTION cs)
{
	EnterCriticalSection(&cs);
	_AuctionGroupInfo* group_info = nullptr;
	bool flag = false;
	Auction_Group_List->SearchStart();

	while (1)
	{
		group_info = Auction_Group_List->SearchData();
		if (group_info == nullptr) break;

		if (group_info->Get_Auction_Info() == info)
		{
			if (group_info->isfull())
			{
				Auction_Group_List->SearchEnd();
				LeaveCriticalSection(&cs);
				return nullptr;
			}

			flag = true;
			break;
		}
	}

	Auction_Group_List->SearchEnd();

	if (!flag)
	{
		group_info = new _AuctionGroupInfo(info);
		Auction_Group_List->Insert(group_info);
	}
	LeaveCriticalSection(&cs);
	return group_info;
}

void _AuctionGruopManager::RemoveAuctionGroup(_AuctionGroupInfo* info, CRITICAL_SECTION cs)
{
	EnterCriticalSection(&cs);
	_AuctionGroupInfo* group_info = nullptr;
	Auction_Group_List->SearchStart();

	while (1)
	{
		group_info = Auction_Group_List->SearchData();
		if (group_info == nullptr) break;
		if (group_info == info)
		{
			Auction_Group_List->Delete(info);
			delete info;
			break;
		}
	}

	Auction_Group_List->SearchEnd();
	LeaveCriticalSection(&cs);
}

_AuctionGroupInfo* _AuctionGruopManager::SearchAuctionGroup(_AuctionInfo* info)
{
	_AuctionGroupInfo* group_info = nullptr;
	Auction_Group_List->SearchStart();

	while (1)
	{
		group_info = Auction_Group_List->SearchData();
		if (group_info == nullptr) break;
		if (group_info->Get_Auction_Info() == info) break;
	}

	Auction_Group_List->SearchEnd();
	return group_info;
}

bool _AuctionGruopManager::CheckAuctionCompleteGroup(_ClientInfo* info)
{
	_AuctionGroupInfo* group_info = SearchAuctionGroup(info->Get_TryAuction()->Get_AuctionInfo());
	group_info->userlist_delete(info);

	if (group_info->get_user_count() == 0)
	{
		Auction_Group_List->Delete(group_info);
		return true;
	}
	return false;
}

void _AuctionGruopManager::ExitAuctionGroup(_ClientInfo* info)
{
	if (info->Get_TryAuction()->Get_AuctionInfo() != nullptr &&
		info->Get_TryAuction()->Get_AuctionInfo()->Get_State() != AUCTION_COMPLETE)
	{
		_AuctionGroupInfo* group_info = SearchAuctionGroup(info->Get_TryAuction()->Get_AuctionInfo());
		group_info->userlist_delete(info);

		if (group_info->GetMaxBidUser() == info)
		{
			group_info->clear_timer_count();
			group_info->ChangeMaxBidUser();
			group_info->searchstart();

			while (1)
			{
				_ClientInfo* client_info;
				if (!group_info->searchdata(client_info)) break;

				client_info->Set_PreState(client_info->Get_State());
				client_info->Set_State(USER_EXIT_INFO_SEND_STAE);
			}

			group_info->searchend();
		}
	}
}

bool _AuctionGruopManager::TryBid(_ClientInfo* info, int price)
{
	_AuctionGroupInfo* group_info = SearchAuctionGroup(info->Get_TryAuction()->Get_AuctionInfo());
	_ClientInfo* user = group_info->GetMaxBidUser();

	if (user == nullptr)
	{
		if (group_info->Get_Auction_Info()->Get_Price() > price)
		{
			return false;
		}

		info->Get_TryAuction()->Set_TryPrice(price);
		group_info->UpdateMaxBidUser(info);
		return true;
	}

	if (user->Get_TryAuction()->Get_TryPrice() > price)
	{
		return false;
	}

	info->Get_TryAuction()->Set_TryPrice(price);
	group_info->UpdateMaxBidUser(info);
	return true;
}