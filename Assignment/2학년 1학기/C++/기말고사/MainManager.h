#pragma once

#include "WindowFrame.h"

#include "LoginMenu.h"
#include "CharacterMenu.h"
#include "GameMenu.h"

// ���� ���� Ŭ����
// ��Ȳ�� �°� �޴� Ŭ������ �ٲ��ش�.
// �̱��� ����

class MainManager {
private:
	static MainManager* pthis;
	MainManager() {}
	~MainManager() {}

	// ���� �޴��� �θ� ��ü
	GameManager* gamemenu;
public:
	// ��ũ�� �޼���
	static MainManager* GetInstance();
	static void Destory();

	// �ٿ� ĳ������ �̿��� ���� ���� ����
	void SetDrawMenu(GameManager* temp) {
		gamemenu = temp;
	}

	// ���� ����� Ŭ������ DrawMenu �� �׷��ش�.
	void DrawMenu(CBackBit* _bit) {
		gamemenu->DrawMenu(_bit);
	}
};