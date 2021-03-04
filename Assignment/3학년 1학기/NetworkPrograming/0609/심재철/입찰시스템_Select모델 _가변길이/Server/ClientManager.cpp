#include "ClientManager.h"
IMPLEMENT_SINGLETON(_ClientManager)

_ClientManager::_ClientManager()
{
	User_List = new CLinkedList<_ClientInfo*>();
}

_ClientInfo* _ClientManager::AddClient(SOCKET _sock, SOCKADDR_IN _addr, CRITICAL_SECTION cs)
{
	EnterCriticalSection(&cs);
	printf("\nClient 접속 : IP = %s, 포트 = %d\n", inet_ntoa(_addr.sin_addr), ntohs(_addr.sin_port));

	_ClientInfo* ptr = new _ClientInfo();
	ptr->Set_Socket(_sock);
	ptr->Set_SockAddr(_addr);
	User_List->Insert(ptr);

	LeaveCriticalSection(&cs);
	return ptr;
}

void _ClientManager::RemoveClient(_ClientInfo* ptr, CRITICAL_SECTION cs)
{
	EnterCriticalSection(&cs);
	closesocket(ptr->Get_Socket());
	printf("\nClient 종료 : IP = %s, 포트 = %d\n", inet_ntoa(ptr->Get_SockAddr().sin_addr), ntohs(ptr->Get_SockAddr().sin_port));

	User_List->Delete(ptr);
	LeaveCriticalSection(&cs);
	delete ptr;
}

_ClientInfo* _ClientManager::SearchClient(const char* id)
{
	_ClientInfo* info = nullptr;
	User_List->SearchStart();
	
	while (1)
	{
		info = User_List->SearchData();
		if (info == nullptr) break;
		if (!strcmp(info->Get_UserInfo()->Get_id(), id)) break;
	}

	User_List->SearchEnd();
	return info;
}