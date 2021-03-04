#ifndef MANAGER_H
#define MANAGER_H
#include "ListManager.h"

class cMain {
private:
	NodeManager* system;
public:
	cMain();
	~cMain();

	void Run();
};

#endif