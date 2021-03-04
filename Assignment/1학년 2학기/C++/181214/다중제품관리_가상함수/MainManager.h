#ifndef MAIN_H
#define MAIN_H

#include "Base.h"
#include "FoodManager.h"
#include "WearManager.h"
#include "ItemManager.h"

class cMain {
private:
	cFoodManager*	mFoodMng;
	cWearManager*	mWearMng;
	cItemManager*	mItemMng;
public:
	cMain();
	~cMain();

	void Initialize();
	int ShowMenu();
	void MajorIuput();
	void MajorOutput();
	void MajorShow();

	void Run();
};
#endif