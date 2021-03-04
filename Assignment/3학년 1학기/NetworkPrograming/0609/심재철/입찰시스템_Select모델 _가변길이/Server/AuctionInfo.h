#pragma once
#include "Global.h"

class _AuctionInfo
{
protected:
	char auction_product[PRODUCT_NAMESIZE];
	int auction_product_code;
	int auction_price;
	int auction_user_count;
	int auction_state;

	_User_Info* max_bid_user;
public:
	_AuctionInfo()
	{
		ZeroMemory(&*this, sizeof(*this));
		max_bid_user = nullptr;
	}

	~_AuctionInfo() {}

	void Set_Product(const char* product) { strcpy(auction_product, product); }
	void Set_Code(const int code) { auction_product_code = code; }
	void Set_Price(const int price) { auction_price = price; }
	void Set_UserCount(const int count) { auction_user_count = count; }
	void Set_State(const int state) { auction_state = state; }

	const char* Get_Product() { return auction_product; }
	const int Get_Code() { return auction_product_code; }
	const int Get_Price() { return auction_price; }
	const int Get_UserCount() { return auction_user_count; }
	const int Get_State() { return auction_state; }

	_User_Info* Get_MaxBidUser() { return max_bid_user; }

	void UserCount_Up() { auction_user_count++; }
	void UserCount_Down() { auction_user_count--; }

	void AuctionComplete(_User_Info* user, int price)
	{
		max_bid_user = user;
		auction_price = price;
		auction_state = AUCTION_COMPLETE;
	}
};