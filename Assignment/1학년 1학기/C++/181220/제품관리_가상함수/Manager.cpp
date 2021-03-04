#include "Manager.h"


CManager::CManager()
{
	
	mProduct_Count = 0;

}
CManager::~CManager()
{

}

int CManager::GetProduct_Count()
{
	return mProduct_Count;
}

bool CManager::CheckData(int _data, int _limit)
{
	if (_data > _limit)
	{
		return false;
	}
	
	return true;
}

