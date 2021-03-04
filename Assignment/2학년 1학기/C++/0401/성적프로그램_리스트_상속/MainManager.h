#pragma once

#include "ProgramingManager.h"
#include "GraphicManager.h"
#include "DesignManager.h"

class MainManager {
private:
	DBManager* DB[4];
public:
	MainManager();
	~MainManager();

	void init();
	void run();

	int Menu();
	void CinCheck();
};