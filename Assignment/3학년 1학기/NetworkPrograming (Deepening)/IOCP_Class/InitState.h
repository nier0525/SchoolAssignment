#pragma once
#include "State.h"

class CInitState : public CState
{
private:
	enum STATE
	{
		INIT_STATE = 1,
	};

	STATE mState;
public:
	CInitState();
	~CInitState();

	void RecvProcess(CClientSection* ptr);
	void SendProcess(CClientSection* ptr);
};