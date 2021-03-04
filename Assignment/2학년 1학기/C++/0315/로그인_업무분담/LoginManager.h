#ifndef LOGINMANAGER_H
#define LOGINMANAGER_H

#include "User.h"

class CLogin {
private:
	CUser* ListUser[USERSIZE];
	CUser* mNowUser;
	int mCount;
public:
	CLogin();
	~CLogin();

	void New();
	bool Login();

	void LogOut();
	void Delete();

	CUser* GetNowUser();
};

#endif