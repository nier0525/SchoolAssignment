#pragma once

#include "Character.h"

// ��ũ Ŭ���� ��� ��ü
class Oak : public Character {
private:

public:
	Oak() : Character() {}
	// ������ ���� �ɷ�ġ ����
	Oak(const char* _name, KindFactory* _kind) : Character(_name, _kind) {
		Character::SetHp(Character::GetHp() + 100);
		Character::SetMp(Character::GetMp() - 50);
		Character::SetDef(Character::GetDef() + 10);
	}
	~Oak() {

	}
};