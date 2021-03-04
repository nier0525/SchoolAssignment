#include "LoginState.h"
#include "ClientSection.h"

CLoginState::CLoginState()
{
	mState = MENU_SELECT_STATE;
}

CLoginState::~CLoginState()
{

}

void CLoginState::RecvProcess(CClientSection* ptr)
{
	switch (mState)
	{
	case MENU_SELECT_STATE:
		switch (ptr->GetProtocol())
		{
		case JOIN_INFO:
			LoginManager::GetInstance()->Join(ptr);
			mState = SEND_RESULT_STATE;
			break;

		case LOGIN_INFO:
			LoginManager::GetInstance()->Login(ptr);
			mState = SEND_RESULT_STATE;
			break;
		}
		break;
	}
}

void CLoginState::SendProcess(CClientSection* ptr)
{
	switch (mState)
	{
	case SEND_RESULT_STATE:
		mState = MENU_SELECT_STATE;
		break;
	}
}