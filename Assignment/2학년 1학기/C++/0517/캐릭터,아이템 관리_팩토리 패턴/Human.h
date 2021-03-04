#pragma once

#include "Character.h"

class Human : public Character {
private:

public:
	Human(const char* _name) : Character(_name) {
		Character::SetHp(Character::GetHp() + 50);
		Character::SetMp(Character::GetMp() + 25);
		Character::SetAtt(Character::GetAtt() + 5);
		Character::SetDef(Character::GetDef() + 5);
		Character::SetBroodCode(HUMAN);
	}
	~Human() {

	}
};