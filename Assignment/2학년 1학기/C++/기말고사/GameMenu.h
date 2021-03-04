#pragma once

#include "GameManager.h"
#include "ItemStore.h"

// ���� �޴� Ŭ����
// ĳ���ͱ��� �����Ѵٸ� ���������� ������ ����(����,�κ��丮,��� ����) ���� ����ϴ� Ŭ�����̴�.
// �̱��� ��ũ��

class GameMenu : public GameManager {
	DECLARE_SINGLETONE(GameMenu)
private:
	// �����ڿ��� �������� �ʱ�ȭ���ش�.
	GameMenu() {
		memset(name, 0, sizeof(name));
		inven = false;
		store = false;
		stats = false;
		
		buy = false;
		sell = false;
		on = false;
	}
	~GameMenu() {}

	char name[CHAR];	// ������ �̸�

	// �� ��Ȳ�� �´� �ο��� ����
	bool inven;
	bool store;
	bool stats;

	bool buy;
	bool sell;
	bool on;
public:

	// ���� �޴�
	void DrawMenu(CBackBit* _bit);

	// �κ��丮 �޴�
	void ShowInven(HDC);
	// ���� �޴�
	void ShowStore(HDC);
	// ���� �޴�
	void ShowStats(HDC);
	// ������ �̸� �Է� �޴�
	void SelectItem(HDC);
	
	// ���� ������
	void SetName(const char* name) { strcpy(this->name, name); }

	// �б� ������
	const char* GetName() { return name; }

	bool Getbuystate() { return buy; }
	bool Getsellstate() { return sell; }
	bool Getonstate() { return on; }
};