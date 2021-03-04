#ifndef CMainManager_H
#define CMainManager_H

#include "Global.h"
#include "CUser.h"
#include "CInGameManager.h"

class CMainManager
{
private:
	CUser*		mUserLst[10];
	int			mUserCount;

	CUser*		mCurrUser;


public:
	CMainManager();
	~CMainManager();

	void Run();
	bool Join();
	bool Login();
	LOGIN_MENU LoginMenu();
	MAIN_MENU MainMenu();
	void Logout();
	void Drop();
	
};
#endif

