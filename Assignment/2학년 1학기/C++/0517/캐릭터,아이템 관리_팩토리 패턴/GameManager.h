#pragma once

#include "CharacterManager.h"
#include "ItemStore.h"

class GameManager {
private:

	static GameManager* pthis;
	GameManager();
	~GameManager();
public:
	static GameManager* GetInstance();
	static void Destory();

	int CharacterMenu();
	int ItemMenu();

	void Init();
	void Run();
};