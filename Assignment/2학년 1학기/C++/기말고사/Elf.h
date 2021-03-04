#pragma once

#include "Character.h"

// ���� Ŭ���� ��� ��ü
class Elf : public Character {
private:

public:
	Elf() : Character() {}
	// ������ ���� �ɷ�ġ ����
	Elf(const char* _name, KindFactory* _kind) : Character(_name, _kind) {
		Character::SetHp(Character::GetHp() - 100);
		Character::SetMp(Character::GetMp() + 50);
		Character::SetAtt(Character::GetAtt() + 10);
	}
	~Elf() {

	}
};