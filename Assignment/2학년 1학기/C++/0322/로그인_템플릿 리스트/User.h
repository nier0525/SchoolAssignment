#pragma once

#include "Global.h"

class CUser {
private:
	char Id[CHARSIZE];
	char Pass[CHARSIZE];
	bool state = false;
public:
	CUser();
	~CUser();
	// 쓰기 함수
	void Setid(const char*);
	void SetPass(const char*);
	// 읽기 함수
	const char* Getid();
	const char* GetPass();
	// state 값 변경
	void Login();
	void LogOut();
};