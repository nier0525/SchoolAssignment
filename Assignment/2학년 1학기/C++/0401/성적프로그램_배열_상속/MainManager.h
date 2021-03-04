#pragma once

#include "Programing.h"
#include "Graphic.h"
#include "Producter.h"

class MainManager {
private:
	CPrograming* Programing;
	CGraphic* Graphic;
	CProducter* Producter;
public:
	MainManager();
	~MainManager();
	
	void CinCheak();

	void init();
	void run();

	int MainMenu();
};