#pragma once

#include "ClientManager.h"
#include "AuctionManager.h"
#include "AuctionGroupManager.h"
#include "ServerManager.h"

DWORD CALLBACK CountThread(LPVOID ptr);

class _MainManager 
{
	DECLARE_SINGLETONE(_MainManager)
private:
	_ClientManager* CM;
	_AuctionManager* AM;
	_AuctionGruopManager* AGM;
	_ServerManager* SM;

	WSADATA wsa;
	FD_SET Rset, Wset;
	CRITICAL_SECTION cs;

	CLinkedList<_User_Info*>* Join_List;

	_MainManager();
	~_MainManager();

public:
	void Init();
	void Run();

	bool Recv_Process(_ClientInfo* ptr);
	bool Send_Process(_ClientInfo* ptr);

	CRITICAL_SECTION Get_CriticalSection() { return cs; }
};