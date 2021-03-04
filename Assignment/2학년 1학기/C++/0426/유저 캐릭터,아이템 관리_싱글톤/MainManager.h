#pragma once

#include "LoginManager.h"
#include "GameManager.h"

class MainManager {
private:
	static MainManager* pthis;
	MainManager();
	~MainManager();
public:
	static MainManager* GetInstance();
	static void Destory();

	void Init();
	void Run();

	int LoginMenu();
};