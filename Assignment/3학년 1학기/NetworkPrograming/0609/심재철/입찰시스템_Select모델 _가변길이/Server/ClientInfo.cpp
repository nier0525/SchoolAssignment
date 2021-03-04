#include "ClientInfo.h"

_ClientInfo::_ClientInfo()
{
	temp_user = new _User_Info();

	pre_state = INIT_STATE;
	state = INIT_STATE;
	loginstate = false;

	recvbyte = comp_recvbyte = sendbyte = comp_sendbyte = 0;
	recv_sizeflag = false;

	ZeroMemory(recv_buf, sizeof(recv_buf));
	ZeroMemory(send_buf, sizeof(send_buf));

	try_auction = new _Try_AuctionInfo();
}


void _ClientInfo::login(_User_Info* info)
{
	loginstate = true;
	user = info;
	ZeroMemory(temp_user, sizeof(_User_Info));
}

void _ClientInfo::logout()
{
	loginstate = false;
	user = nullptr;
}

void _ClientInfo::auction_start(_AuctionInfo* info)
{
	try_auction->Set_AuctionInfo(info);
	try_auction->Set_Result(AUCTION_FAIL);
}

void _ClientInfo::auction_end()
{
	try_auction->Set_AuctionInfo(nullptr);
	try_auction->Set_Result(NODATA);
	try_auction->Set_TryPrice(0);
}

void _ClientInfo::try_auction_success()
{
	try_auction->Set_Result(AUCTION_SUCCESS);
	user->Set_AuctionMoney(user->Get_AuctionMoney() - try_auction->Get_TryPrice());
}

bool _ClientInfo::check_auction_success()
{
	if (try_auction->Get_Result() == AUCTION_SUCCESS) return true;
	return false;
}

bool _ClientInfo::check_money(const int price)
{
	if (user->Get_AuctionMoney() < price) return false;
	return true;
}

void _ClientInfo::deposit(const int price)
{
	user->Set_AuctionMoney(user->Get_AuctionMoney() + price);
}

bool _ClientInfo::check_send_complete()
{
	if (sendbyte == 0) return true;
	return false;
}

bool _ClientInfo::is_disconnected()
{
	if (state == DISCONNECTED_STATE) return true;
	return false;
}