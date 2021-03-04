#include "CharacterManager.h"

// ĳ���� ���� ��� �޼ҵ�
Character* CharacterFactory::MakeCharacter(const char* _name) {
	KindFactory* kind = SelectKind();	// ���� �Լ��� ���� �ڽ� ��ü���� ���õ� ���� ���丮 Ŭ������ ����
	Character* ptr = CreateCharacter(_name, kind);	// ĳ���� ����

	return ptr;	// ������ ĳ���� ��ȯ
}