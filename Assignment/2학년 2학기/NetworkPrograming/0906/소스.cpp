#pragma comment(lib, "ws2_32")
#include <stdio.h>
#include <winsock2.h>

#define TESTNAME "www.youtube.com"

#define ERRISNAME "gethostbyname()"
#define ERRISADDR "gethostbyaddr()"

typedef struct in6_addr
{
	union {
		u_char Byte[16];
		u_short Word[8];
	} u;
}IN6_ADDR;

// 소켓 함수 오류 출력
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// 도메인 이름 -> IPv4 주소
BOOL GetIPAddr(char *name, IN_ADDR *addr, int *index)
{
	HOSTENT *ptr = gethostbyname(name);
	if (ptr == NULL) {
		err_display((char*)ERRISNAME);
		return FALSE;
	}
	if (ptr->h_addrtype != AF_INET)
		return FALSE;

	while(1) {
		if (ptr->h_addr_list[*index] != nullptr) {
			*index += 1;
		}
		else {
			break;
		}
	}

	for (int i = 0; i < *index; i++) {
		memcpy((addr + i), ptr->h_addr_list[i], ptr->h_length);
	}
	
	return TRUE;
}

// IPv6 주소 -> 도메인 이름(현재 호스트 이름)
BOOL GetDomainName(IN6_ADDR addr, char *name, int namelen)
{
	HOSTENT *ptr = gethostbyaddr((char *)&addr, sizeof(addr), AF_INET6);
	if (ptr == NULL) {
		err_display((char*)ERRISADDR);
		return FALSE;
	}
	if (ptr->h_addrtype != AF_INET6)
		return FALSE;
	strncpy(name, ptr->h_name, namelen);
	return TRUE;
}

int main(int argc, char *argv[])
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	printf("도메인 이름(변환 전) = %s\n", TESTNAME);

	// 도메인 이름 -> IP 주소
	
	IN_ADDR addr[50];

	int index = 0;
	IN6_ADDR addr6;
	memset(&addr6, 0, sizeof(addr6));

	if (GetIPAddr((char*)TESTNAME, addr, &index)) {
		// 성공이면 결과 출력
		
		for (int i = 0; i < index; i++) {
			printf("IP 주소(변환 후) = %s\n", inet_ntoa(addr[i]));
		}
		// IP 주소 -> 도메인 이름
		char name[256];

		if (GetDomainName(addr6, name, sizeof(name))) {
			// 성공이면 결과 출력
			printf("도메인 이름(다시 변환 후) = %s\n", name);
		}
	}

	WSACleanup();
	return 0;
}