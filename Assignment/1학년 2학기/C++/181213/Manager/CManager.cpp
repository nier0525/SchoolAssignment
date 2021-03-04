#include "Manager.h"

CManager::CManager()
{
	memset(mStudentList, 0, sizeof(mStudentList));
	mStuCount = 0;
}
CManager::~CManager()
{
	for (int i = 0; i < mStuCount; i++)
	{
		delete mStudentList[i];
	}
}