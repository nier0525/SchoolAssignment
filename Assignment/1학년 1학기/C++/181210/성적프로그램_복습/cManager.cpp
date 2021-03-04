#include "Manager.h"

cManager::cManager() {
	memset(mStudentList, 0, sizeof(mStudentList));
	mStuCount = 0;
}

cManager::~cManager() {
	for (int i = 0; i < mStuCount; i++)
		delete mStudentList[i];
}