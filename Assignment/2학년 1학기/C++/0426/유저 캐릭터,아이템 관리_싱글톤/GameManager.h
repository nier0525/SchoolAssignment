#pragma once

#include "CharacterManager.h"
#include "ItemStore.h"
#include "LoginManager.h"

class GameManager {
private:
	static GameManager* pthis;
	GameManager();
	~GameManager();
public:
	static GameManager* GetInstance();
	static void Destory();

	void Init();
	void Run();

	int GameMenu();
};