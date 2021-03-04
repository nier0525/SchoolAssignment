#pragma once
#include "TCPSocket.h"

struct QueueSendData
{
	//int send_id;

	int sendbytes;
	int comp_sendbytes;

	char sendbuf[BUFSIZE];
};

class CPacking : public CTCPSocket
{
protected:
	int recvbytes;
	int comp_recvbytes;

	char recvbuf[BUFSIZE];

	bool recv_sizeflag;

	queue<QueueSendData*>* sendbuf;
public:
	CPacking();
	CPacking(SOCKET _sock);
	~CPacking();

	PROTOCOL GetProtocol();
	const char* UnPackData();

	bool Recv();
	int CompleteRecv(int _completebyte);

	void PackingData(PROTOCOL protocol, const char* buf, int _size);

	bool Send();
	int CompleteSend(int _completebyte);

	
};