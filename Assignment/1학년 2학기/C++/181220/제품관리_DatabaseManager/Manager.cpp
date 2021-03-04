#include "Manager.h"


CManager::CManager()
{
	
}
CManager::~CManager()
{

}



bool CManager::CheckData(int _data, int _limit)
{
	if (_data > _limit)
	{
		return false;
	}
	
	return true;
}

