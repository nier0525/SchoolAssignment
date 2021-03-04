#include "CMainManager.h"

CMainManager::CMainManager()
{
	memset(mUserLst, 0, sizeof(mUserLst));
	mUserCount = 0;

}

CMainManager::~CMainManager()
{
	for (int i = 0; i < mUserCount; i++)
	{
		if (mUserLst[i] != nullptr)
		{
			delete mUserLst[i];
			mUserLst[i] = nullptr;
		}
	}
}

void CMainManager::Run()
{
	CInGameManager* ingame_manager = new CInGameManager();

	while (1)
	{
		bool endflag = false;		

		MAIN_MENU select = MainMenu();

		switch (select)
		{
		case JOIN:
			Join();
			break;
		case LOGIN:
			if (Login())
			{
				while (1)
				{
					bool endflag = false;
					LOGIN_MENU select = LoginMenu();
					
					switch (select)
					{
					case GAMESTART:
						ingame_manager->GameStart(mCurrUser);
						ingame_manager->GameRun();
						ingame_manager->GameOver();
						break;
					case LOGOUT:
						Logout();
						endflag = true;
						break;
					case DROP:
						Drop();
						endflag = true;
						break;
					}
					if (endflag)
					{
						break;
					}
				}				
			}			
			break;
		case EXIT:
			endflag = true;
			break;
		default:
			cout << "잘못입력했습니다" << endl;
			continue;
		}

		if (endflag)
		{
			break;
		}
	}

	delete ingame_manager;
}

bool CMainManager::Join()
{
	char id[10];
	char pw[10];

	cout << "ID:";
	cin >> id;

	bool flag = false;
	for (int i = 0; i < mUserCount; i++)
	{
		if (strcmp(mUserLst[i]->GetId(), id) == 0)
		{
			flag = true;
			break;
		}
	}

	if (flag)
	{
		cout << "이미 존재하는 아이디입니다" << endl;
		return false;
	}

	cout << "PW:";
	cin >> pw;

	mUserLst[mUserCount++] = new CUser(id, pw);
	cout << mUserLst[mUserCount];
	return true;

}

bool CMainManager::Login()
{
	char id[10];
	char pw[10];


	cout << "ID:";
	cin >> id;

	bool flag = false;
	for (int i = 0; i < mUserCount; i++)
	{
		if (strcmp(mUserLst[i]->GetId(), id) == 0)
		{
			flag = true;
			break;
		}
	}

	if (!flag)
	{
		cout << "없는 아이디입니다" << endl;
		return false;
	}

	cout << "PW:";
	cin >> pw;

	flag = false;
	for (int i = 0; i < mUserCount; i++)
	{
		if (strcmp(mUserLst[i]->GetPw(), pw) == 0)
		{
			flag = true;
			mCurrUser = mUserLst[i];
			mCurrUser->SetLogin();
			break;
		}
	}

	if (!flag)
	{
		cout << "패스워드가 틀렸습니다" << endl;
		return false;
	}

	return true;

}

MAIN_MENU CMainManager::MainMenu()
{
	MAIN_MENU select;	

	while (1)
	{
		cout << "<메뉴>" << endl;
		cout << "1.회원가입" << endl;
		cout << "2.로그인" << endl;
		cout << "3.종료" << endl;
		cout << "선택:";


		cin >> select;

		if (select < JOIN || select>3)
		{
			cout << "잘못입력했습니다" << endl;
			continue;
		}
		break;
	}

	return select;
}

LOGIN_MENU CMainManager::LoginMenu()
{
	LOGIN_MENU select;

	while (1)
	{		
		cout << "<메뉴>" << endl;
		cout << "1.게임시작" << endl;
		cout << "2.로그아웃" << endl;
		cout << "3.회원탈퇴" << endl;	
		cout << "선택:";

		
		cin >> select;

		if (select<1 || select>3)
		{
			cout << "잘못입력했습니다" << endl;
			continue;
		}
		break;
	}

	return select;
	
}

void CMainManager::Logout()
{
	mCurrUser->SetLogout();
}

void CMainManager::Drop()
{
	for (int i = 0; i < mUserCount; i++)
	{
		if (mUserLst[i] == mCurrUser)
		{
			delete mUserLst[i];
			for (int j = i; j < mUserCount - 1; j++)
			{
				mUserLst[j] = mUserLst[j + 1];
			}
			mUserLst[mUserCount - 1] = nullptr;
			break;
		}
	}

	mUserCount--;

}