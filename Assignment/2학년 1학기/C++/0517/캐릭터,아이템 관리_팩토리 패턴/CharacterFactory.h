#pragma once

#include "Elf.h"
#include "Oak.h"
#include "Human.h"

class CharacterFactory {
private:
	static CharacterFactory* pthis;
	CharacterFactory();
	~CharacterFactory();
public:
	static CharacterFactory* GetInstance();
	static void Destory();

	Character* NewCharacter(const char*, int, int);
};