#pragma once

#include "CharacterFactory.h"
#include "ElfItemFactory.h"

// ���� ���丮 ���� Ŭ����
// �̱��� ��ũ��

class ElfFactory : public CharacterFactory {
	DECLARE_SINGLETONE(ElfFactory)
private:
	ElfFactory() {}
	~ElfFactory() {}

	// �θ� ��ü�� �Լ��� ��������
	virtual Character* CreateCharacter(const char*, KindFactory*);
	virtual KindFactory* SelectKind();
public:

};