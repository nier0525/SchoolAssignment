#ifndef MANAGER_H
#define MANAGER_H
#include "Base.h"
#include "Food.h"
#include "Item.h"
#include "Wear.h"

class cManager {
protected:
	cFood*	mFoodList[MAXSIZE];
	int mFoodCount;
	cWear*	mWearList[MAXSIZE];
	int mWearCount;
	cItem*	mItemList[MAXSIZE];
	int mItemCount;
public:
	cManager();
	~cManager();
};

#endif