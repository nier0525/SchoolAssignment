
#include "MainManager.h"

int main()
{
	CMainManager* mainprocess = new CMainManager();
	mainprocess->Run();

	delete mainprocess;
	return 0;
}