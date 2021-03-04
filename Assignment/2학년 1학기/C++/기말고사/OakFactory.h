#pragma once

#include "CharacterFactory.h"
#include "OakItemFactory.h"

// ��ũ ���丮 ���� Ŭ����
// �̱��� ��ũ��

class OakFactory : public CharacterFactory {
	DECLARE_SINGLETONE(OakFactory)
private:
	OakFactory() {}
	virtual ~OakFactory() {}

	// �θ� ��ü�� �Լ��� ��������
	virtual Character* CreateCharacter(const char*, KindFactory*);
	virtual KindFactory* SelectKind();
public:

};

