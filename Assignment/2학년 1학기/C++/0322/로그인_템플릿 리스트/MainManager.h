#pragma once

#include "LoginManager.h"

class Manager {
private:
	LoginManager* LM;
public:
	Manager();
	~Manager();

	void Run();
	void init();

	int MainMeun();
	int LoginMenu();
};