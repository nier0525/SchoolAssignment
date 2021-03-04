#pragma once

#include "ProgramingManager.h"
#include "DesignManager.h"
#include "GraphicManager.h"

class CManager {
private:
	CSchoolManager* Total[4];
public:
	CManager();
	~CManager();

	void init();

	void Run();
	void InputMenu();
	void DisplayMenu();
	void SearchMenu();

	int Menu();
	int SubMenu();
};