#pragma once

#include "User.h"
#include "ListNode.h"

class LoginManager {
private:
	ListNode<CUser*>* ListUser;
	Node<CUser*>* NowUser;
public:
	LoginManager();
	~LoginManager();

	void init();
	void New();
	bool Login();
	void Logout();
	void Del();

	// User 데이터 중복 체크 함수
	Node<CUser*>* UserCheak(CUser*);
};