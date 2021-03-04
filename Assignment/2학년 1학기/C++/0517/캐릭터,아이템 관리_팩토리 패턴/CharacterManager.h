#pragma once

#include "Character.h"

class CharacterManager {
private:
	static const char* brood[];
	static const char* job[];

	static CharacterManager* pthis;
	CharacterManager();
	~CharacterManager();

	Character* NowCharacter;
public:
	static CharacterManager* GetInstance();
	static void Destory();

	void New();
	void Delete();
	bool SelectCharacter();
	Character* GetNowCharacter();
	void PrintCharacter(Character*);
};