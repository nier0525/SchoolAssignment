#pragma once

#include "Character.h"

class Oak : public Character {
private:

public:
	Oak(const char* _name) : Character(_name) {
		Character::SetHp(Character::GetHp() + 100);
		Character::SetMp(Character::GetMp() - 50);
		Character::SetDef(Character::GetDef() + 10);
		Character::SetBroodCode(OAK);
	}
	~Oak() {

	}
};