#pragma once

#include "Item.h"
#define WEAPON_CODE 201		// ������ �ڵ�

class Character;

class Wenpon : public Item {
private:
	// �ɷ�ġ �� �Ϸ� �ڵ�
	int Att;
	static int Num;
public:
	Wenpon(const char*, int, int); // �̸�, ����, �ɷ�ġ, �ɷ�ġ2
	~Wenpon();

	// �б� ������
	virtual int GetFirstAbility();
	virtual int GetSecordAbility();

	// �ڵ� ����
	virtual void MakeCode();
	// ������ ����,���� �� ĳ���� ���� ���� ���
	virtual bool OnItem(Character*);
	virtual bool OffItem(Character*);
};