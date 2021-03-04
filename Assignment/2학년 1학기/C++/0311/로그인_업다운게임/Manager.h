#ifndef MANAGER_H
#define MANAGER_H

#include "User.h"

class Manager {
private:
	CUser* lstuser[USERSIZE];
	CUser* now;
	GManager* GM;
	int conut;
public:
	Manager();
	~Manager();

	void NewLog();
	bool Login();

	void Logout();
	void Del();

	void Run();
};

#endif