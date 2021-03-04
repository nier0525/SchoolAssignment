#pragma once
#include "State.h"
#include "LoginManager.h"

class CLoginState : public CState
{
private:
	enum STATE
	{
		MENU_SELECT_STATE = 1,
		SEND_RESULT_STATE,
	};

	STATE mState;
public:
	CLoginState();
	~CLoginState();

	void RecvProcess(CClientSection* ptr);
	void SendProcess(CClientSection* ptr);
};