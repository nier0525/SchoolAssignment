#pragma once

#include "GameManager.h"

// ĳ���� �޴� Ŭ����
// �̱��� ����

class CharacterMenu : public GameManager {
private:
	static CharacterMenu* pthis;
	// ���� ������ �ʱ�ȭ
	CharacterMenu() {
		memset(name, 0, sizeof(name));
		New = false;
		Sel_Oak = false;
		Sel_Elf = false;
		Select = false;
		Remove = false;
	}
	~CharacterMenu() {}

	char name[CHAR];	// ĳ���� �̸� �Է�

	// �� ��Ȳ�� ���� �ο��� ����
	bool New;
	bool Sel_Oak;
	bool Sel_Elf;
	bool Select;
	bool Remove;
public:
	static CharacterMenu* GetInstance();
	static void Destory();

	// ���� ������
	void SetName(const char* _name) { strcpy(name, _name); }

	// �޴� ȣ��
	void DrawMenu(CBackBit* _bit);
	// ĳ���� ���� �޴� ȣ��
	void CharacterSeMenu(HDC hdc, CBackBit* _bit);


	// �б� ������
	const char* GetName() { return name; }

	bool GetNewState() { return New; }
	bool GetSelect_Oak() { return Sel_Oak; }
	bool GetSelect_Elf() { return Sel_Elf; }
	bool GetSelect() { return Select; }
	bool GetRemove() { return Remove; }
};