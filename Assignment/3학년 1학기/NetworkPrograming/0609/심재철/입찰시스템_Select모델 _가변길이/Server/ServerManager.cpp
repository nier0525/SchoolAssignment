#include "ServerManager.h"
IMPLEMENT_SINGLETON(_ServerManager)

_ServerManager::_ServerManager()
{
	sock_info = new _SocketInfo();
}

void _ServerManager::Init()
{
	sock_info->Set_Event(CreateEvent(nullptr, false, false, nullptr));
	sock_info->Set_ServerSock(socket(PF_INET, SOCK_STREAM, 0));
	if (sock_info->Get_ServerSock() == INVALID_SOCKET)
		err_quit("socket()");

	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(9000);

	sock_info->Set_ServerAddr(addr);
	if (bind(sock_info->Get_ServerSock(), (SOCKADDR*)&sock_info->Get_ServerAddr(), 
		sizeof(sock_info->Get_ServerAddr())) == SOCKET_ERROR)
		err_quit("bind()");

	sock_info->Set_DummySock(socket(PF_INET, SOCK_DGRAM, 0));
	if (sock_info->Get_DummySock() == INVALID_SOCKET)
		err_quit("socket()");

	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(9900);

	sock_info->Set_DummyAddr(addr);
	if (bind(sock_info->Get_DummySock(), (SOCKADDR*)&sock_info->Get_DummyAddr(),
		sizeof(sock_info->Get_DummyAddr())) == SOCKET_ERROR)
		err_quit("bind()");

	if (listen(sock_info->Get_ServerSock(), 5) == SOCKET_ERROR)
		err_quit("listen()");
}

void _ServerManager::err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
}

void _ServerManager::err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void _ServerManager::GetProtocol(const char* ptr, PROTOCOL& protocol)
{
	memcpy(&protocol, ptr, sizeof(PROTOCOL));
}

void _ServerManager::PackPacket(char* buf, PROTOCOL protocol, int& size)
{
	char* ptr = buf;
	size = 0;

	ptr = ptr + sizeof(size);

	memcpy(ptr, &protocol, sizeof(protocol));
	ptr = ptr + sizeof(protocol);
	size = size + sizeof(protocol);

	ptr = buf;
	memcpy(ptr, &size, sizeof(size));

	size = size + sizeof(size);
}

void _ServerManager::PackPacket(char* buf, PROTOCOL protocol, const int data, int& size)
{
	char* ptr = buf;
	size = 0;

	ptr = ptr + sizeof(size);

	memcpy(ptr, &protocol, sizeof(protocol));
	ptr = ptr + sizeof(protocol);
	size = size + sizeof(protocol);

	memcpy(ptr, &data, sizeof(data));
	ptr = ptr + sizeof(data);
	size = size + sizeof(data);

	ptr = buf;
	memcpy(ptr, &size, sizeof(size));

	size = size + sizeof(size);
}

void _ServerManager::PackPacket(char* buf, PROTOCOL protocol, int data, const char* str1, int& size)
{
	char* ptr = buf;
	int strsize1 = strlen(str1);
	size = 0;

	ptr = ptr + sizeof(size);

	memcpy(ptr, &protocol, sizeof(protocol));
	ptr = ptr + sizeof(protocol);
	size = size + sizeof(protocol);

	memcpy(ptr, &data, sizeof(data));
	ptr = ptr + sizeof(data);
	size = size + sizeof(data);

	memcpy(ptr, &strsize1, sizeof(strsize1));
	ptr = ptr + sizeof(strsize1);
	size = size + sizeof(strsize1);

	memcpy(ptr, str1, strsize1);
	ptr = ptr + strsize1;
	size = size + strsize1;

	ptr = buf;
	memcpy(ptr, &size, sizeof(size));

	size = size + sizeof(size);
}

void _ServerManager::PackPacket(char* _buf, PROTOCOL _protocol, CLinkedList<_AuctionInfo*>* _list, int& _size)
{
	char* ptr = _buf;
	_size = 0;
	int count = _list->GetCount();

	ptr = ptr + sizeof(_size);

	memcpy(ptr, &_protocol, sizeof(_protocol));
	ptr = ptr + sizeof(_protocol);
	_size = _size + sizeof(_protocol);

	char* countptr = ptr;
	ptr = ptr + sizeof(count);
	_size = _size + sizeof(count);

	_list->SearchStart();

	while (1)
	{
		_AuctionInfo* info = _list->SearchData();
		if (info == nullptr)
		{
			break;
		}

		if (info->Get_State() == AUCTION_COMPLETE)
		{
			count--;
			continue;
		}

		int code = info->Get_Code();
		memcpy(ptr, &code, sizeof(code));
		ptr = ptr + sizeof(code);
		_size = _size + sizeof(code);


		char product[PRODUCT_NAMESIZE];
		ZeroMemory(product, sizeof(product));
		strcpy(product, info->Get_Product());

		int namesize = strlen(product);
		memcpy(ptr, &namesize, sizeof(namesize));
		ptr = ptr + sizeof(namesize);
		_size = _size + sizeof(namesize);

		memcpy(ptr, product, namesize);
		ptr = ptr + namesize;
		_size = _size + namesize;

		int price = info->Get_Price();
		memcpy(ptr, &price, sizeof(price));
		ptr = ptr + sizeof(price);
		_size = _size + sizeof(price);
	}

	_list->SearchEnd();

	memcpy(countptr, &count, sizeof(count));

	ptr = _buf;
	memcpy(ptr, &_size, sizeof(_size));

	_size = _size + sizeof(_size);
}

void _ServerManager::PackPacket(char* _buf, PROTOCOL _protocol, const char* _str1, int& _size)
{
	char* ptr = _buf;
	int strsize1 = strlen(_str1);
	_size = 0;

	ptr = ptr + sizeof(_size);

	memcpy(ptr, &_protocol, sizeof(_protocol));
	ptr = ptr + sizeof(_protocol);
	_size = _size + sizeof(_protocol);

	memcpy(ptr, &strsize1, sizeof(strsize1));
	ptr = ptr + sizeof(strsize1);
	_size = _size + sizeof(strsize1);

	memcpy(ptr, _str1, strsize1);
	ptr = ptr + strsize1;
	_size = _size + strsize1;

	ptr = _buf;
	memcpy(ptr, &_size, sizeof(_size));

	_size = _size + sizeof(_size);
}

void _ServerManager::UnPackPacket(const char* _buf, char* _str1, char* _str2, char* _str3, int& _data)
{
	int str1size, str2size, str3size;

	const char* ptr = _buf + sizeof(PROTOCOL);

	memcpy(&str1size, ptr, sizeof(str1size));
	ptr = ptr + sizeof(str1size);

	memcpy(_str1, ptr, str1size);
	ptr = ptr + str1size;

	memcpy(&str2size, ptr, sizeof(str2size));
	ptr = ptr + sizeof(str2size);

	memcpy(_str2, ptr, str2size);
	ptr = ptr + str2size;

	memcpy(&str3size, ptr, sizeof(str3size));
	ptr = ptr + sizeof(str3size);

	memcpy(_str3, ptr, str3size);
	ptr = ptr + str3size;

	memcpy(&_data, ptr, sizeof(_data));
	ptr = ptr + sizeof(_data);
}

void _ServerManager::UnPackPacket(const char* _buf, char* _str1, char* _str2)
{
	int str1size, str2size;

	const char* ptr = _buf + sizeof(PROTOCOL);

	memcpy(&str1size, ptr, sizeof(str1size));
	ptr = ptr + sizeof(str1size);

	memcpy(_str1, ptr, str1size);
	ptr = ptr + str1size;

	memcpy(&str2size, ptr, sizeof(str2size));
	ptr = ptr + sizeof(str2size);

	memcpy(_str2, ptr, str2size);
	ptr = ptr + str2size;
}

void _ServerManager::UnPackPacket(const char* _buf, int& _data1)
{
	const char* ptr = _buf + sizeof(PROTOCOL);

	memcpy(&_data1, ptr, sizeof(_data1));
	ptr = ptr + sizeof(_data1);
}

void _ServerManager::UnPackPacket(const char* _buf, int& _data1, int& _data2)
{
	const char* ptr = _buf + sizeof(PROTOCOL);

	memcpy(&_data1, ptr, sizeof(_data1));
	ptr = ptr + sizeof(_data1);

	memcpy(&_data2, ptr, sizeof(_data2));
	ptr = ptr + sizeof(_data2);
}

void _ServerManager::UnPackPacket(const char* _buf, _AuctionInfo& _info)
{
	int strsize;

	const char* ptr = _buf + sizeof(PROTOCOL);

	memcpy(&strsize, ptr, sizeof(strsize));
	ptr = ptr + sizeof(strsize);

	char product[PRODUCT_NAMESIZE];
	ZeroMemory(product, sizeof(product));
	memcpy(product, ptr, strsize);
	ptr = ptr + strsize;

	int count = 0;
	memcpy(&count, ptr, sizeof(count));
	ptr = ptr + sizeof(count);

	_info.Set_Product(product);
	_info.Set_UserCount(count);
}

int _ServerManager::MessageRecv(_ClientInfo* _info)
{
	int retval = recv(_info->Get_Socket(), _info->Set_RecvBuffer() + _info->Get_Comp_RecvByte(),
		_info->Get_RecvByte() - _info->Get_Comp_RecvByte(), 0);

	if (retval == SOCKET_ERROR) return ERROR_DISCONNECTED;
	else if (retval == 0) return DISCONNECTED;
	else
	{
		_info->Set_Comp_RecvByte(_info->Get_Comp_RecvByte() + retval);
		
		if (_info->Get_Comp_RecvByte() == _info->Get_RecvByte())
		{
			_info->Set_Comp_RecvByte(0);
			_info->Set_RecvByte(0);
			return SOC_TRUE;
		}
		return SOC_FALSE;
	}
}

int _ServerManager::MessageSend(_ClientInfo* _info)
{
	int retval = send(_info->Get_Socket(), _info->Get_SendBuffer() + _info->Get_Comp_SendByte(),
		_info->Get_SendByte() - _info->Get_Comp_SendByte(), 0);
	if (retval == SOCKET_ERROR) return ERROR_DISCONNECTED;
	else if (retval == 0) return DISCONNECTED;
	else
	{
		_info->Set_Comp_SendByte(_info->Get_Comp_SendByte() + retval);

		if (_info->Get_SendByte() == _info->Get_Comp_SendByte())
		{
			_info->Set_SendByte(0);
			_info->Set_Comp_SendByte(0);

			return SOC_TRUE;
		}
		return SOC_FALSE;
	}
}

int _ServerManager::PacketRecv(_ClientInfo* ptr)
{
	if (!ptr->Get_Recv_Sizeflag())
	{
		ptr->Set_RecvByte(sizeof(int));
		int retval = MessageRecv(ptr);

		switch (retval)
		{
		case SOC_TRUE:
			int byte;
			memcpy(&byte, ptr->Get_RecvBuffer(), sizeof(int));
			ptr->Set_RecvByte(byte);
			ptr->Recv_Sizeflag_Complete();
			return SOC_FALSE;

		case SOC_FALSE:
			return SOC_FALSE;

		case ERROR_DISCONNECTED:
			return DISCONNECTED;

		case DISCONNECTED:
			return DISCONNECTED;
		}
	}

	int retval = MessageRecv(ptr);
	switch (retval)
	{
	case SOC_TRUE:
		ptr->Recv_Sizeflag_NotComplete();
		return SOC_TRUE;

	case SOC_FALSE:
		return SOC_FALSE;

	case ERROR_DISCONNECTED:
		return DISCONNECTED;

	case DISCONNECTED:
		return DISCONNECTED;
	}
}

int _ServerManager::SendCheck(_ClientInfo* ptr)
{
	int result = MessageSend(ptr);

	switch (result)
	{
	case ERROR_DISCONNECTED:
		err_display("connect end");
	case DISCONNECTED:
		ptr->Set_State(DISCONNECTED_STATE);
		return DISCONNECTED;

	case SOC_FALSE:
		return SOC_FALSE;

	case SOC_TRUE:
		return SOC_TRUE;
	}
}