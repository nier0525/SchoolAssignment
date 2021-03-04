#ifndef WEARMANAGER_H
#define WEARMANAGER_H
#include "Wear.h"
#include "Manager.h"

class cWearManager : public cManager {
private:
	static const char*	mWearSex[];
	static const char*	mWearName[];
	cWear*	mWear[MAXSIZE];
	int mWearCount;
public:
	cWearManager();
	~cWearManager();

	void Input();
	void Output();
	void Show();
};

#endif