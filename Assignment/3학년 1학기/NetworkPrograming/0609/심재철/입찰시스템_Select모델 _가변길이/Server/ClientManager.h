#pragma once
#include "Global.h"
#include "ClientInfo.h"

class _ClientManager
{
	DECLARE_SINGLETONE(_ClientManager)
private:
	CLinkedList<_ClientInfo*>* User_List;

	_ClientManager();
	~_ClientManager()
	{
		if (User_List != nullptr) delete User_List;
	}
public:
	_ClientInfo* AddClient(SOCKET _sock, SOCKADDR_IN _addr, CRITICAL_SECTION cs);
	_ClientInfo* SearchClient(const char* id);
	void RemoveClient(_ClientInfo* ptr, CRITICAL_SECTION cs);

	CLinkedList<_ClientInfo*>* Get_UserList() { return User_List; }
};