#pragma once

#include "Item.h"
#define ARMOR_CODE 202	// ������ �ڵ�

class Character;

class Armor : public Item {
private:
	// �ɷ�ġ �� �Ϸ� �ڵ�
	int Def;
	static int Num;
public:
	Armor(const char*, int, int); // �̸�, ����, �ɷ�ġ, �ɷ�ġ2
	~Armor();

	// �б� ������
	virtual int GetFirstAbility();
	virtual int GetSecordAbility();

	// �ڵ� ����
	virtual void MakeCode();
	// ������ ����,���� �� ĳ���� ���� ���� ���
	virtual bool OnItem(Character*);
	virtual bool OffItem(Character*);
};