#pragma once

#include "ListNode.h"
#include "User.h"

class LoginManager {
private:
	ListNode<User*>* ListUser;
	Node<User*>* SearchUser;
	User* NowUser;

	static LoginManager* Pthis;
	LoginManager();
	~LoginManager();
public:
	static LoginManager* GetInstance();
	static void Destory();

	User* GetNowUser();
	void Init();

	void New();
	bool Login();
	void Logout();
	void Delete();
};