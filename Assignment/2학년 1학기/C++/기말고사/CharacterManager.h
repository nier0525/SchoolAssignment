#pragma once

#include "LoginManager.h"
#include "OakFactory.h"
#include "ElfFactory.h"

// ĳ���͸� ������ �Ŵ��� Ŭ����
// �̱��� ��ũ��

// ĳ���� ����, ����, ���� ��� �������� ĳ���Ϳ� ���õ� ���� ����ϴ� Ŭ����

class CharacterManager {
	DECLARE_SINGLETONE(CharacterManager)
private:
	CharacterManager() {}
	~CharacterManager() {}

	// ���� �������� ĳ���͸� ���� ����
	Character* NowCharacter;
public:

	void Init();	// �ʱ�ȭ �Լ�
	bool CreateCharacter(const char*, CharacterFactory*);	// ����
	bool SelectCharacter(const char*);						// ����
	bool DropCharacter(const char*);						// ����

	void PrintCharacter(HDC, int, int);						// ĳ���� �ɷ�ġ ���
	void ShowNowCharacterInven(HDC, int, int);				// ���� �������� ĳ������ �κ��丮 ���� ������ ���

	Character* GetNowCharacter() { return NowCharacter; }	// ���� �������� ĳ���� �б� ������
};