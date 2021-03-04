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
			cout << "�߸��Է��߽��ϴ�" << endl;
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
		cout << "�̹� �����ϴ� ���̵��Դϴ�" << endl;
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
		cout << "���� ���̵��Դϴ�" << endl;
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
		cout << "�н����尡 Ʋ�Ƚ��ϴ�" << endl;
		return false;
	}

	return true;

}

MAIN_MENU CMainManager::MainMenu()
{
	MAIN_MENU select;	

	while (1)
	{
		cout << "<�޴�>" << endl;
		cout << "1.ȸ������" << endl;
		cout << "2.�α���" << endl;
		cout << "3.����" << endl;
		cout << "����:";


		cin >> select;

		if (select < JOIN || select>3)
		{
			cout << "�߸��Է��߽��ϴ�" << endl;
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
		cout << "<�޴�>" << endl;
		cout << "1.���ӽ���" << endl;
		cout << "2.�α׾ƿ�" << endl;
		cout << "3.ȸ��Ż��" << endl;	
		cout << "����:";

		
		cin >> select;

		if (select<1 || select>3)
		{
			cout << "�߸��Է��߽��ϴ�" << endl;
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