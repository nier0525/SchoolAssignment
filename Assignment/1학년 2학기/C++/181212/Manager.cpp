#include "Manager.h"

cManager::cManager() {
	for (int i = 0; i < MAXSIZE; i++) {
		mFoodList[i] = nullptr;
		mWearList[i] = nullptr;
		mItemList[i] = nullptr;
	}
	mFoodCount = 0;
	mWearCount = 0;
	mItemCount = 0;
}

cManager::~cManager() {
	for (int i = 0; i < MAXSIZE; i++) {
		delete mFoodList[i];
		delete mWearList[i];
		delete mItemList[i];
	}
}