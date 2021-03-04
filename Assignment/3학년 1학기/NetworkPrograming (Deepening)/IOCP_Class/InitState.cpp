#include "InitState.h"
#include "ClientSection.h"
#include "LoginState.h"

CInitState::CInitState()
{
	mState = INIT_STATE;
}

CInitState::~CInitState()
{

}

void CInitState::RecvProcess(CClientSection* ptr)
{
	switch (mState)
	{
	case INIT_STATE:
		printf("Init\n");
		ptr->SetState(ptr->GetLoginState());
		break;
	}
}

void CInitState::SendProcess(CClientSection* ptr)
{

}