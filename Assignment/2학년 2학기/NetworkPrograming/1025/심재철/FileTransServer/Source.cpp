#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFSIZE 4096
#define FILENAMESIZE 256
#define MAXCONCLIENT 100

#define INTRO_MSG "������ ������ �����ϼ���"
#define FILE_EXIST_MSG "�����ϰ��� �ϴ� ������ �̹� ������ �����ϴ� �����Դϴ�."

enum PROTOCOL
{
	INTRO = 1,
	FILE_INFO,
	FILE_TRANS_DENY,
	FILE_TRANS_START_POINT,
	FILE_TRANS_DATA,

	FILE_TRANS_CONTINE,
	FILE_EXIT
};

enum DENY_CODE
{
	FILEEXIST = -1
};

struct _File_info
{
	char filename[FILENAMESIZE];
	int  filesize;	
	int  nowsize;
};


struct _ClientInfo
{
	SOCKET sock;
	SOCKADDR_IN addr;
	_File_info  file_info;
	char packetbuf[BUFSIZE];
};

void err_quit(const char*);
void err_display(const char*);

int recvn(SOCKET, char*, int, int);

_ClientInfo* AddClientInfo(SOCKET sock, SOCKADDR_IN addr);
void ReMoveClientInfo(_ClientInfo*);

bool SearchFile(const char*);
bool PacketRecv(SOCKET, char*);
PROTOCOL GetProtocol(const char*);
int PackPacket(char*, PROTOCOL, const char*);
int PackPacket(char*, PROTOCOL, int, const char*);
int PackPacket(char*, PROTOCOL, int);



void UnPackPacket(const char* _buf, char* _str1, int& _data1);
void UnPackPacket(const char*, int&, char*);

_ClientInfo* ClientInfo[MAXCONCLIENT];
int count;

int main(int argc, char* argv[])
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// ������ ��ſ� ����� ����

	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	
	
	char buf[BUFSIZE];
	FILE *fp=nullptr;

	while (1)
	{
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			continue;
		}	

		_ClientInfo* ClientPtr = AddClientInfo(client_sock, clientaddr);		
		
		int size=PackPacket(ClientPtr->packetbuf, INTRO, INTRO_MSG);

		retval = send(ClientPtr->sock, ClientPtr->packetbuf, size, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			ReMoveClientInfo(ClientPtr);
			continue;
		}
		
		bool fileflag = false;
		bool endflag = false;

		while (1)
		{
			if (!PacketRecv(ClientPtr->sock, ClientPtr->packetbuf))
			{
				break;
			}

			PROTOCOL protocol=GetProtocol(ClientPtr->packetbuf);

			switch (protocol)
			{
			case FILE_INFO:
				{
					char filename[FILENAMESIZE];
					int filesize;
					memset(filename, 0, sizeof(filename));
					
					UnPackPacket(ClientPtr->packetbuf, filename, filesize);

					printf("-> ���� ���� �̸�: %s\n", filename);
					printf("-> ���� ���� ũ��: %d\n", filesize);

					if (SearchFile(filename))
					{
						fp = fopen(filename, "rb");
						fseek(fp, 0, SEEK_END);
						int fsize = ftell(fp);
						fclose(fp);

						if (filesize == fsize)
						{
							printf("�����ϴ� ���� ���� �䱸\n");

							int size=PackPacket(ClientPtr->packetbuf, FILE_TRANS_DENY, FILEEXIST, FILE_EXIST_MSG);

							retval = send(ClientPtr->sock, ClientPtr->packetbuf, size, 0);
							if (retval == SOCKET_ERROR)
							{
								err_display("send()");
							}
							break;
						}

						else
						{
							strcpy(ClientPtr->file_info.filename, filename);
							ClientPtr->file_info.filesize = filesize;
							ClientPtr->file_info.nowsize = fsize;
						}
					}

					else
					{
						strcpy(ClientPtr->file_info.filename, filename);
						ClientPtr->file_info.filesize = filesize;
						ClientPtr->file_info.nowsize = 0;
					}			
					
					int size=PackPacket(ClientPtr->packetbuf, FILE_TRANS_START_POINT, ClientPtr->file_info.nowsize);

					retval = send(ClientPtr->sock, ClientPtr->packetbuf, size, 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						endflag = true;
						break;
					}
				}
				break;

			case FILE_TRANS_DATA:
				{
					if (!fileflag)
					{
						fileflag = true;
						fp = fopen(ClientPtr->file_info.filename, "ab");
					}

					int transsize;
					UnPackPacket(ClientPtr->packetbuf, transsize, buf);
					fwrite(buf, 1, transsize, fp);
					if (ferror(fp)) {
						perror("���� ����� ����");
						endflag = true;
						break;
					}
					ClientPtr->file_info.nowsize += transsize;
				}
				break;				

			case FILE_TRANS_CONTINE:
				if (fileflag && fp != nullptr)
				{
					fileflag = false;
					fclose(fp);
				}

				int fsize;
				UnPackPacket(ClientPtr->packetbuf, fsize, (char*)&ClientPtr->file_info);
				ZeroMemory(&ClientPtr->file_info, sizeof(fsize));

				int size;
				size	= PackPacket(ClientPtr->packetbuf, INTRO, INTRO_MSG);

				retval = send(ClientPtr->sock, ClientPtr->packetbuf, size, 0);
				if (retval == SOCKET_ERROR)
				{
					err_display("send()");
					break;
				}
				break;

			case FILE_EXIT:
				endflag = true;
				break;
			}//switch end

			if (ClientPtr->file_info.filesize != 0 && ClientPtr->file_info.filesize == ClientPtr->file_info.nowsize && !endflag)
			{
				printf("���ۿϷ�!!\n");
			}

			if (endflag)
			{
				printf("���α׷� ����\n");
				break;
			}

		}//inner while end

		if (fileflag && fp != nullptr)
		{
			fclose(fp);
		}

		ReMoveClientInfo(ClientPtr);
	}//outer while end

	 // closesocket()
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;
}

// ���� �Լ� ���� ��� �� ����
void err_quit(const char *msg)
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

// ���� �Լ� ���� ���
void err_display(const char *msg)
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


_ClientInfo* AddClientInfo(SOCKET sock, SOCKADDR_IN addr)
{
	_ClientInfo* ptr = new _ClientInfo;
	ZeroMemory(ptr, sizeof(_ClientInfo));
	ptr->sock = sock;
	memcpy(&ptr->addr, &addr, sizeof(addr));
	ClientInfo[count++] = ptr;

	printf("\nFileSender ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));

	return ptr;
}

void ReMoveClientInfo(_ClientInfo* ptr)
{
	
	printf("FileSender ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));

	for (int i = 0; i<count; i++)
	{
		if (ClientInfo[i] == ptr)
		{
			closesocket(ptr->sock);
			delete ptr;
			for (int j = i; j<count - 1; j++)
			{
				ClientInfo[j] = ClientInfo[j + 1];
			}
			break;
		}
	}

	count--;
}

bool SearchFile(const char *filename)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFindFile = FindFirstFile(filename, &FindFileData);
	if (hFindFile == INVALID_HANDLE_VALUE)
		return false;
	else {
		FindClose(hFindFile);
		return true;
	}
}
// ����� ���� ������ ���� �Լ�
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

bool PacketRecv(SOCKET _sock, char* _buf)
{
	int size;

	int retval = recvn(_sock, (char*)&size, sizeof(size), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("gvalue recv error()");
		return false;
	}
	else if (retval == 0)
	{
		return false;
	}

	retval = recvn(_sock, _buf, size, 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("gvalue recv error()");
		return false;

	}
	else if (retval == 0)
	{
		return false;
	}

	return true;
}

int PackPacket(char* _buf, PROTOCOL  _protocol, const char* _str) //��Ʈ�� 
{
	char* ptr = _buf;
	int strsize = strlen(_str);
	int size = 0;

	ptr = ptr + sizeof(size);

	memcpy(ptr, &_protocol, sizeof(_protocol));
	ptr = ptr + sizeof(_protocol);
	size = size + sizeof(_protocol);

	
	memcpy(ptr, &strsize, sizeof(strsize));
	ptr = ptr + sizeof(strsize);
	size = size + sizeof(strsize);

	memcpy(ptr, _str, strsize);
	ptr = ptr + strsize;
	size = size + strsize;

	ptr = _buf;
	memcpy(ptr, &size, sizeof(size));

	size = size + sizeof(size);
	return size;
}

int PackPacket(char* _buf, PROTOCOL  _protocol, int _data, const char* _str) //�������� �ź� ����
{
	char* ptr = _buf;
	int strsize = strlen(_str);
	int size = 0;

	ptr = ptr + sizeof(size);

	memcpy(ptr, &_protocol, sizeof(_protocol));
	ptr = ptr + sizeof(_protocol);
	size = size + sizeof(_protocol);

	memcpy(ptr, &_data, sizeof(_data));
	ptr = ptr + sizeof(_data);
	size = size + sizeof(_data);

	memcpy(ptr, &strsize, sizeof(strsize));
	ptr = ptr + sizeof(strsize);
	size = size + sizeof(strsize);

	memcpy(ptr, _str, strsize);
	ptr = ptr + strsize;
	size = size + strsize;

	ptr = _buf;
	memcpy(ptr, &size, sizeof(size));

	size = size + sizeof(size);
	return size;
}


int PackPacket(char* _buf, PROTOCOL _protocol, int _data)//���� ���� ��ġ ����
{
	char* ptr = _buf;
	int size = 0;
	
	ptr = ptr + sizeof(size);
	
	memcpy(ptr, &_protocol, sizeof(_protocol));
	ptr = ptr + sizeof(_protocol);
	size = size + sizeof(_protocol);

	memcpy(ptr, &_data, sizeof(_data));
	ptr = ptr + sizeof(_data);
	size = size + sizeof(_data);

	ptr = _buf;
	memcpy(ptr, &size, sizeof(size));

	size = size + sizeof(size);
	return size;
}

PROTOCOL GetProtocol(const char* _buf)
{
	PROTOCOL protocol;
	memcpy(&protocol, _buf, sizeof(PROTOCOL));
	return protocol;
}

void UnPackPacket(const char* _buf, char* _str1, int& _data1)//�����̸� & ���� ũ��
{
	const char* ptr = _buf + sizeof(PROTOCOL);
	int strsize;

	memcpy(&strsize, ptr, sizeof(strsize));
	ptr = ptr + sizeof(strsize);

	memcpy(_str1, ptr, strsize);
	ptr = ptr + strsize;

	memcpy(&_data1, ptr, sizeof(_data1));
	ptr = ptr + sizeof(_data1);	
}

void UnPackPacket(const char* _buf, int& _size, char* _targetbuf) //���� ���� ������
{
	const char* ptr = _buf + sizeof(PROTOCOL);

	memcpy(&_size, ptr, sizeof(_size));
	ptr = ptr + sizeof(_size);

	memcpy(_targetbuf, ptr, _size);
}
