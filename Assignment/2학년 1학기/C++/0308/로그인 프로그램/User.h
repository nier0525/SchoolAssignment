#ifndef USER_H
#define USER_H

#include "Global.h"

class CUser {
private:
	char Id[NAMESIZE];
	char Pass[PASSSIZE];
	bool state = false;
public:
	CUser(const char* _id, const char* _pass);
	~CUser();

	void Setid(const char* _id);
	const char* Getid();

	void SetPass(const char* _pass);
	const char* GetPass();

	void Login();
	void LogOut();
};

#endif