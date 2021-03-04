#pragma once
#include "Packing.h"
#include "State.h"

class CInitState;
class CLoginState;

class CClientSection : public CPacking
{
protected:
	UserInfo* user;
	bool login_state;

	CState* mState;

	CInitState* mInitState;
	CLoginState* mLoginState;
public:
	CClientSection();
	CClientSection(SOCKET _sock);
	~CClientSection();

	void SetState(CState* state)
	{
		mState = state;
	}

	CState* GetState() { return mState; }
	CLoginState* GetLoginState() { return mLoginState; }

	void Login(UserInfo* _user);
	void Logout();

	const UserInfo* GetUser() { return user; }
	bool IsLogin() { return login_state; }
};