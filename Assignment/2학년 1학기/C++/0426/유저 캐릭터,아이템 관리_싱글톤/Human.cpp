#include "Human.h"

Human::Human(const char* _name) : Character(_name) {
	Character::SetHP(Character::GetHP() + 50);
	Character::SetMP(Character::GetMP() + 25);
	Character::SetAtt(Character::GetAtt() + 5);
	Character::SetDef(Character::GetDef() + 2);
	Character::SetBroodCode(HUMAN);
}

Human::~Human() {

}