#include "CMainManager.h"

int main()
{
	CMainManager* manager = new CMainManager();
	manager->Run();
	delete manager;

	return 0;
}