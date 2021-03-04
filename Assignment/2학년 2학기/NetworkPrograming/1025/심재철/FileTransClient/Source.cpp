// argv[1] = tera.jpg , argv[2] = mabinogi.jpg

#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFSIZE 4096
#define MAX_FILE_READ_SIZE 2048
#define FILENAMESIZE 256

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
}Fileinfo;


void err_quit(const char *msg);
void err_display(const char *msg);
int recvn(SOCKET s, char *buf, int len, int flags);

bool PacketRecv(SOCKET, char*);

int PackPacket(char* _buf, PROTOCOL _protocol, const char* _str1, int _data);
int PackPacket(char*, PROTOCOL,int, const char*);
//int PackPacket(char*, PROTOCOL, _File_info*);

PROTOCOL GetProtocol(const char*);

void UnPackPacket(const char*, int&, char*);
void UnPackPacket(const char*, char*);
void UnPackPacket(const char*, int&);

int main(int argc, char* argv[])
{
	int retval;
	char buf[BUFSIZE];
	
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	ZeroMemory(&Fileinfo, sizeof(Fileinfo));

	FILE* fp = nullptr;
	bool endflag = false;

	while (1)
	{
		if (!PacketRecv(sock, buf))
		{
			break;
		}

		PROTOCOL protocol=GetProtocol(buf);

		switch (protocol)
		{
		case INTRO:		
			{				
				char msg[BUFSIZE];	

				memset(msg, 0, sizeof(msg));
				UnPackPacket(buf, msg);
				printf("%s\n\n", msg);

				int index = 1;

				while (argv[index] != nullptr) {
					printf("%d . %s\n", index, argv[index]);
					index++;
				}

				printf("\n���� : ");
				scanf("%d", &index);

				strcpy(Fileinfo.filename, argv[index]);

				fp = fopen(Fileinfo.filename, "rb");

				if (!fp)
				{
					err_quit("fopen");
				}

				fseek(fp, 0, SEEK_END);
				Fileinfo.filesize = ftell(fp);
				fclose(fp);

				int size=PackPacket(buf, FILE_INFO, Fileinfo.filename, Fileinfo.filesize);

				retval = send(sock, buf, size, 0);
				if (retval == SOCKET_ERROR)
				{
					err_quit("file info send()");
				}
			}

			break;
		case FILE_TRANS_DENY:
			{
				char msg[BUFSIZE];
				int deny_code;
				memset(msg, 0, sizeof(msg));
				UnPackPacket(buf, deny_code, msg);
				switch (deny_code)
				{
				case FILEEXIST:
					printf("%s\n", msg);
					endflag = true;
					break;
				}
			}
			
			break;
		case FILE_TRANS_START_POINT:
			{
				
				UnPackPacket(buf, Fileinfo.nowsize);

				fp = fopen(Fileinfo.filename, "rb");
				fseek(fp, Fileinfo.nowsize, SEEK_SET);

				while (1)
				{
					char rbuf[BUFSIZE];
					int trans_size = fread(rbuf, 1, MAX_FILE_READ_SIZE, fp);
					if (trans_size > 0)
					{
						system("cls");

						int size=PackPacket(buf, FILE_TRANS_DATA, trans_size, rbuf);
						retval = send(sock, buf, size, 0);
						if (retval == SOCKET_ERROR)
						{
							err_display("send()");
							break;
						}
						Fileinfo.nowsize += trans_size;
						printf("\n������ : %d / %d", Fileinfo.nowsize , Fileinfo.filesize);
						Sleep(200);
					}

					else if (trans_size == 0 && Fileinfo.nowsize == Fileinfo.filesize)
					{
						printf("\n���� ���� �Ϸ�!: %d ����Ʈ\n", Fileinfo.filesize);
						endflag = true;
						break;
					}
					else
					{
						perror("���� ����� ����");
						break;
					}
				}

				fclose(fp);

			}			
			break;
		}

		if (endflag)
		{
			printf("\n�ٸ� ������ ���� �ϰڽ��ϱ�?\n\n");
			printf("1. ��\n");
			printf("2. �ƴϿ�\n\n");
			printf("���� : ");

			char index[128];
			scanf("%s", &index);

			index[2] = '\0';

			switch (atoi(index)) {
			case 1:
				protocol = FILE_TRANS_CONTINE;
				break;;

			default:
				protocol = FILE_EXIT;
				break;
			}

			int fsize = 0;
			int size = 0;

			ZeroMemory(&Fileinfo, sizeof(Fileinfo));
			fsize = sizeof(_File_info);

			size = PackPacket(buf, protocol, fsize, (char*)&Fileinfo);
			retval = send(sock, buf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("send()");
				break;
			}

			if (protocol == FILE_TRANS_CONTINE) {
				endflag = false;
				system("cls");
				continue;
			}
			else {
				break;
			}
		}
	}
	// closesocket()
	closesocket(sock);

	// ���� ����
	WSACleanup();
	system("pause");
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

int PackPacket(char* _buf, PROTOCOL _protocol, const char* _str1, int _data)
{
	char* ptr = _buf;
	int strsize = strlen(_str1);
	int size = 0;

	ptr = ptr + sizeof(size);

	memcpy(ptr, &_protocol, sizeof(_protocol));
	ptr = ptr + sizeof(_protocol);
	size = size + sizeof(_protocol);

	memcpy(ptr, &strsize, sizeof(strsize));
	ptr = ptr + sizeof(strsize);
	size = size + sizeof(strsize);

	memcpy(ptr, _str1, strsize);
	ptr = ptr + strsize;
	size = size + strsize;

	memcpy(ptr, &_data, sizeof(_data));
	ptr = ptr + sizeof(_data);
	size = size + sizeof(_data);

	ptr = _buf;

	memcpy(ptr, &size, sizeof(size));

	size = size + sizeof(size);

	return size;
}

int PackPacket(char* _buf, PROTOCOL _protocol, int _datasize, const char* _filedata)
{
	char* ptr = _buf;
	int size = 0;

	ptr = ptr + sizeof(size);

	memcpy(ptr, &_protocol, sizeof(_protocol));
	ptr = ptr + sizeof(_protocol);
	size = size + sizeof(_protocol);

	memcpy(ptr, &_datasize, sizeof(_datasize));
	ptr = ptr + sizeof(_datasize);
	size = size + sizeof(_datasize);

	memcpy(ptr, _filedata, _datasize);
	ptr = ptr + _datasize;
	size = size + _datasize;

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

void UnPackPacket(const char* _buf, int& _data, char* _str)
{
	const char* ptr = _buf;
	int strsize;

	ptr = ptr + sizeof(PROTOCOL);

	memcpy(&_data, ptr, sizeof(_data));
	ptr = ptr + sizeof(_data);
	
	memcpy(&strsize, ptr, sizeof(strsize));
	ptr = ptr + sizeof(strsize);

	memcpy(_str, ptr, strsize);
}

void UnPackPacket(const char* _buf, char* _str)
{
	const char* ptr = _buf;
	int strsize;

	ptr = ptr + sizeof(PROTOCOL);

	memcpy(&strsize, ptr, sizeof(strsize));
	ptr = ptr + sizeof(strsize);

	memcpy(_str, ptr, strsize);
}

void UnPackPacket(const char* _buf, int& _data)
{
	const char* ptr = _buf + sizeof(PROTOCOL);

	memcpy(&_data, ptr, sizeof(_data));
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