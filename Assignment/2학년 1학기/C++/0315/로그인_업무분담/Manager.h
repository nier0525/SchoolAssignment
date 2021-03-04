#ifndef MANAGER_H
#define MANAGER_H

#include "LoginManager.h"
#include "GameManager.h"

class Manager {
private:
	CLogin* LOG;
	GManager* GM;
public:
	Manager();
	~Manager();

	void init();
	void Run();
};

#endif