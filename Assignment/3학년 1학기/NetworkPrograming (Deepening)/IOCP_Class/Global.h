#pragma once
#pragma warning (disable : 4996)

#include <WinSock2.h>
#pragma comment (lib, "ws2_32")

#include <mysql.h>
#pragma comment (lib, "libmysql.lib")

#include <stdio.h>
#include <stdlib.h>

#define PORT 9050
#define BUFSIZE 4096
#define MSGSIZE 128

#include <vector>
#include <list>
#include <queue>
using namespace std;

#define DB_HOST "localhost"	// DB IP주소 또는 도메인 또는 localhost
#define DB_USER "root"		// DB접속 계정 명
#define DB_PASS "1234"		// DB접속 계정 암호
#define DB_NAME "userdb"	// DB 이름

#define JOIN_SUCCESS_MSG "\n회원가입에 성공했습니다.\n"
#define ERROR_JOIN_MSG "\n이미 있는 ID 입니다.\n"
#define LOGIN_SUCCESS_MSG "\n로그인에 성공했습니다.\n"
#define ERROR_LOGIN_ID_MSG "\n해당 ID 는 없는 ID 입니다.\n"
#define ERROR_LOGIN_PW_MSG "\nPW 가 일치하지 않습니다.\n"
#define ERROR_LOGIN_EXISTS_MSG "\n다른 호스트에서 접속중인 계정입니다.\n"


enum PROTOCOL : INT64
{
	JOIN = 1,
	LOGIN,
	EXIT,
	JOIN_INFO,
	JOIN_RESULT,
	LOGIN_INFO,
	LOGIN_RESULT,
};

enum STATE
{
	MENU_SELECT_STATE = 0,
	RESULT_SEND_STATE,
	JOIN_STATE,
	LOGIN_STATE,
};

enum RESULT
{
	NODATA = -1,
	JOIN_SUCCESS = 1,
	ERROR_JOIN_EXISTS,
	LOGIN_SUCCESS,
	ERROR_LOGIN_ID,
	ERROR_LOGIN_PW,
	ERROR_LOGIN_EXISTS
};

enum IO_TYPE
{
	IO_RECV = 1,
	IO_SEND,
	IO_ACCEPT,
};

enum
{
	SOC_ERROR = -1,
	SOC_TRUE,
	SOC_FALSE,
};

enum UserInfo_Data
{
	ID = 0,
	PW,
	NAME
};

struct WSAOverLapped_EX
{
	WSAOVERLAPPED overlapped;
	LPVOID ptr;
	IO_TYPE type;
};

struct UserInfo
{
	char id[MSGSIZE];
	char pw[MSGSIZE];

	char name[MSGSIZE];

	UserInfo() 
	{
		memset(this, 0, sizeof(*this));
	}
	UserInfo(char* _id, char* _pw, char* _name)
	{
		strcpy(id, _id);
		strcpy(pw, _pw);
		strcpy(name, _name);
	}
};







#define MAKE_NO_COPY(CLASSNAME)                                             \
        private:                                                            \
               CLASSNAME(const CLASSNAME&){}                                \
               CLASSNAME& operator=(const CLASSNAME&);

// 싱클톤 패턴 생성 매크로
#define DECLARE_SINGLETONE(CLASSNAME)                                       \
        MAKE_NO_COPY(CLASSNAME)                                             \
        private:                                                            \
			   static CLASSNAME* mPthis;									\
        public:                                                             \
               static CLASSNAME* Create();									\
			   static CLASSNAME* GetInstance();								\
               static void Destroy();										 

// 싱글톤 패턴 구현 매크로

#define IMPLEMENT_SINGLETON(CLASSNAME)                              \
               CLASSNAME* CLASSNAME::mPthis= NULL;					\
                                                                    \
               CLASSNAME* CLASSNAME::Create()						\
			   {													\
                       if(mPthis == NULL)							\
						{											\
                              mPthis=new CLASSNAME();				\
						}											\
                        return mPthis;								\
               }                                                    \
               CLASSNAME* CLASSNAME::GetInstance()					\
               {                                                    \
                    return mPthis;									\
               }													\
			   void CLASSNAME::Destroy()							\
               {													\
					if(mPthis) delete mPthis;						\
			   }																