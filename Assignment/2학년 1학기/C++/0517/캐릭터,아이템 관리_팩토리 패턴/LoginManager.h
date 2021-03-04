#pragma once

#include "User.h"

class LoginManager {
private:
	ListNode<User*>* UserList;
	Node<User*>* SearchUser;
	User* NowUser;

	static LoginManager* pthis;
	LoginManager();
	~LoginManager();
public:
	static LoginManager* GetInstance();
	static void Destory();

	User* GetNowUser();
	void Init();

	void New();
	bool Login();
	void LogOut();
	void Delete();
};