#pragma once

#include "Elf.h"
#include "Oak.h"
#include "Human.h"

class CharacterManager {
private:
	static const char* brood[];
	static CharacterManager* pthis;
	CharacterManager();
	~CharacterManager();
public:
	static CharacterManager* GetInstance();
	static void Destory();

	void Init();
	void NewChar();
	void SelChar();
	void DelChar();
	void PrintChar(Character* index);
};