#include "MainManager.h"

int main(void)
{
	CMainManager* manager = new CMainManager();
	manager->Initialize();
	manager->Run();
	delete manager;
	return 0;
}