#include "Elf.h"

Elf::Elf(const char* _name) : Character(_name) {
	Character::SetHP(Character::GetHP() - 100);
	Character::SetMP(Character::GetMP() + 25);
	Character::SetAtt(Character::GetAtt() + 10);
	Character::SetBroodCode(ELF);
}

Elf::~Elf() {

}