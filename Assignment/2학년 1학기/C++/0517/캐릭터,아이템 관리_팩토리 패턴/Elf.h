#pragma once

#include "Character.h"

class Elf : public Character {
private:

public:
	Elf(const char* _name) : Character(_name) {
		Character::SetHp(Character::GetHp() - 100);
		Character::SetMp(Character::GetMp() + 50);
		Character::SetAtt(Character::GetAtt() + 10);
		Character::SetBroodCode(ELF);
	}
	~Elf() {

	}
};