#include "MainManager.h"

int main()
{
	_MainManager::GetInstance()->Init();
	_MainManager::GetInstance()->Run();
	_MainManager::Destroy();
	return 0;
}