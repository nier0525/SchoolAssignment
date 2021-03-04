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
	// ���� �Լ�
	void Setid(const char*);
	void SetPass(const char*);
	// �б� �Լ�
	const char* Getid();
	const char* GetPass();
	// state �� ����
	void Login();
	void LogOut();
};