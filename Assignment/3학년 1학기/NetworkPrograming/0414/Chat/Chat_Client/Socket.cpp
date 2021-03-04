#include "Socket.h"

// 소켓 함수 오류 출력
void err_quit(const char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	ExitProcess(0);
}

void err_display(const char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(nullptr, (char*)lpMsgBuf, msg, MB_OK);
	LocalFree(lpMsgBuf);
}

// sendto 소켓 초기화
void S_SocketInit(SOCKET& sock, SOCKADDR_IN& addr, int _ttl) {
	int retval;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) {
		err_quit("socket()");
	}

	// 멀티캐스트 TTL 설정
	int ttl = _ttl;
	retval = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL,
		(char *)&ttl, sizeof(ttl));
	if (retval == SOCKET_ERROR) err_quit("setsockopt()");

	ZeroMemory(&addr, sizeof(SOCKADDR_IN));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(IP);
	addr.sin_port = htons(PORT);
}

// sendto 어드레스 주소 변경
void S_AddrChange(SOCKADDR_IN& addr) {
	addr.sin_addr.s_addr = inet_addr(IP);
	addr.sin_port = htons(PORT);
}

void S_AddrChange(SOCKADDR_IN& addr, const char* ip) {
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_port = htons(PORT + 1);
}

// recv 소켓 초기화
void R_SocketInit(SOCKET& sock, SOCKADDR_IN& addr) {
	int retval;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) {
		err_quit("socket()");
	}

	BOOL optval = TRUE;
	retval = setsockopt(sock, SOL_SOCKET,
		SO_REUSEADDR, (char *)&optval, sizeof(optval));
	if (retval == SOCKET_ERROR) err_quit("setsockopt()");

	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(PORT + 1);
	retval = bind(sock, (SOCKADDR*)&addr, sizeof(addr));
	if (retval == SOCKET_ERROR) {
		err_quit("bind()");
	}
}

// 멀티캐스트 그룹 가입
bool Multicast_Group_Add(SOCKET& sock, ip_mreq& mreq, const char* ip) {
	int retval;

	// 멀티캐스트 그룹 가입
	mreq.imr_multiaddr.s_addr = inet_addr(ip);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	retval = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
		(char *)&mreq, sizeof(mreq));
	if (retval == SOCKET_ERROR) { 
		err_display("setsockopt()"); 
		return false;
	}

	return true;
}

// 멀티캐스트 그룹 탈퇴
void Multicast_Group_Drop(SOCKET& sock, ip_mreq mreq) {
	int retval;

	// 멀티캐스트 그룹 탈퇴
	retval = setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP,
		(char *)&mreq, sizeof(mreq));
	if (retval == SOCKET_ERROR) { 
		err_display("setsockopt()"); 
	}
}

// 소켓 정리
void CleanSocket(SocketPakage* arg) {
	closesocket(arg->s_sock);
	closesocket(arg->r_sock);
	WSACleanup();
}