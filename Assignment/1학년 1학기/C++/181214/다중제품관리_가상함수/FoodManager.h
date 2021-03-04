#ifndef FOODMANAGER_H
#define FOODMANAGER_H
#include "Food.h"
#include "Manager.h"

class cFoodManager : public cManager {
private:
	static const char*	mFoodName[];
	cFood*	mFood[MAXSIZE];
	int mFoodCount;
public:
	cFoodManager();
	~cFoodManager();

	void Input();
	void Output();
	void Show();
};

#endif