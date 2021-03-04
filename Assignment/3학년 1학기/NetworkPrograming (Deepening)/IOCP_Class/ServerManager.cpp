#include "ServerManager.h"

CServerManager* CServerManager::pthis = nullptr;

CServerManager* CServerManager::GetInstance()
{
	CMySQLDBManager::Create();
	ClientManager::Create();
	LoginManager::Create();

	if (!pthis) pthis = new CServerManager();
	return pthis;
}

void CServerManager::Destory()
{
	if (pthis) delete pthis;

	CMySQLDBManager::Destroy();
	CError::Destory();
	ClientManager::Destroy();
	LoginManager::Destroy();
}

CServerManager::CServerManager()
{
	mListenSock = nullptr;
}

CServerManager::~CServerManager()
{
	if (mListenSock != nullptr)
		delete mListenSock;
}

bool CServerManager::Begin()
{
	// MySQL DB Setting
	CMySQLDBManager::GetInstance()->Begin();

	// WSA Setting
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return false;

	// IOCP Setting
	if (!BeginIOCP()) return false;

	// Listen Setting
	mListenSock = new CListenSocket();
	mListenSock->TCP_Setting(nullptr, PORT);
	mListenSock->Listen();

	// Manager Setting
	ClientManager::GetInstance()->Begin();
	LoginManager::GetInstance()->Begin();

	return true;
}

void CServerManager::Run()
{
	while (1)
	{
		SOCKET sock = mListenSock->Accept();
		if (sock == INVALID_SOCKET)
		{
			CError::GetInstance()->err_display("Accept");
			break;
		}

		RegisterIOCP(sock, 0);

		WSAOverLapped_EX* wsa_accept = new WSAOverLapped_EX;
		memset(wsa_accept, 0, sizeof(WSAOverLapped_EX));

		wsa_accept->ptr = (void*)sock;
		wsa_accept->type = IO_ACCEPT;

		PostQueuedCompletionStatus(hcp, 0, sock, (LPOVERLAPPED)wsa_accept);
	}
}

void CServerManager::End()
{
	CMySQLDBManager::GetInstance()->End();
	ClientManager::GetInstance()->End();
	LoginManager::GetInstance()->End();
	WSACleanup();
	Destory();
}

bool CServerManager::Recv(void* ptr, int len)
{
	CClientSection* client = (CClientSection*)ptr;
	if (client == nullptr)
	{
		CError::GetInstance()->err_display("Recv");
		return true;
	}

	int result = client->CompleteRecv(len);

	switch (result)
	{
	case SOC_ERROR:
	case SOC_FALSE:
		return false;
	case SOC_TRUE:
		break;
	}

	client->GetState()->RecvProcess(client);

	if (!client->Recv())
	{
		return false;
	}

	return true;
}

bool CServerManager::Send(void* ptr, int len)
{
	CClientSection* client = (CClientSection*)ptr;
	if (client == nullptr)
	{
		CError::GetInstance()->err_display("Recv");
		return true;
	}

	int result = client->CompleteSend(len);

	switch (result)
	{
	case SOC_ERROR:
	case SOC_FALSE:
		return false;
	case SOC_TRUE:
		break;
	}

	client->GetState()->SendProcess(client);
	return true;
}

void CServerManager::Accept(void* ptr)
{
	printf("Accept\n");

	WSAOverLapped_EX* overlapped = (WSAOverLapped_EX*)ptr;
	SOCKET sock = (SOCKET)overlapped->ptr;
	// Add
	CClientSection* client = ClientManager::GetInstance()->AddClient(sock);
	delete overlapped;

	if (!client->Recv())
	{
		return;
	}
}

void CServerManager::Disconneted(void* ptr)
{
	printf("Disconneted\n");

	// Remove
	ClientManager::GetInstance()->RemoveClient((CClientSection*)ptr);
}