#include "Oak.h"

Oak::Oak(const char* _name) : Character(_name) {
	Character::SetHP(Character::GetHP() + 100);
	Character::SetMP(Character::GetMP() - 25);
	Character::SetDef(Character::GetDef() + 10);
	Character::SetBroodCode(OAK);
}

Oak::~Oak() {

}