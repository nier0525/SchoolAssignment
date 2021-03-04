#include "ClientSection.h"
#include "InitState.h"
#include "LoginState.h"

CClientSection::CClientSection() 
{
	user = nullptr;
	login_state = false;

	mInitState = new CInitState();
	mLoginState = new CLoginState();

	mState = mInitState;
}

CClientSection::CClientSection(SOCKET _sock) : CPacking(_sock)
{
	user = nullptr;
	login_state = false;

	mInitState = new CInitState();
	mLoginState = new CLoginState();

	mState = mInitState;
}

CClientSection::~CClientSection() 
{
	if (user != nullptr)	delete user;

	if (mInitState != nullptr) delete mInitState;

	if (mLoginState != nullptr) delete mLoginState;
}

void CClientSection::Login(UserInfo* _user)
{
	CLock lock;

	user = _user;
	login_state = true;
}

void CClientSection::Logout()
{
	CLock lock;

	user = nullptr;
	login_state = false;
}