#pragma once

#include "FoodManager.h"
#include "ItemManager.h"
#include "WearManager.h"

class CMainManager {
private:
	static const char* ProductList[];
	CManager* Total[4];
public:
	CMainManager();
	~CMainManager();

	void init();
	void run();

	CManager* Menu();
	int MainMenu();
};