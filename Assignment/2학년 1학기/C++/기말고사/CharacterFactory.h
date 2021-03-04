#pragma once

#include "Elf.h"
#include "Oak.h"
#include "KindFactory.h"

// ���丮 ���� ���

class CharacterFactory {
protected:
	// ���� ���� �Լ��̱� ������ �ڽ� ��ü���� �������ش�.
	virtual Character* CreateCharacter(const char* , KindFactory*) = 0;
	virtual KindFactory* SelectKind() = 0;
public:
	CharacterFactory() {}
	virtual ~CharacterFactory() {}

	Character* MakeCharacter(const char* _name); // ĳ���� ���� ��� �޼ҵ�
};