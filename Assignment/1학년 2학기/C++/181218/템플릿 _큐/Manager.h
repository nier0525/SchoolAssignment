#ifndef MANAGER_H
#define MANAGER_H
#include "QueueManager.h"

class cManager {
private:
	cQManager* cQ;
public:
	cManager();
	~cManager();

	int SetMenu();
	void run();
};

#endif