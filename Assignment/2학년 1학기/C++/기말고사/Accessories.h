#pragma once

#include "Item.h"
#define ACCESSORIES_CODE 203	// ������ �ڵ�

class Character;

class Accessories : public Item {
private:
	// �ɷ�ġ �� �Ϸ� �ڵ�
	int HP;	
	int MP;
	static int Num;
public:
	Accessories(const char*, int, int, int); // �̸�, ����, �ɷ�ġ, �ɷ�ġ2
	~Accessories();

	// �б� ������
	virtual int GetFirstAbility();
	virtual int GetSecordAbility();

	// �ڵ� ����
	virtual void MakeCode();
	// ������ ����,���� �� ĳ���� ���� ���� ���
	virtual bool OnItem(Character*);
	virtual bool OffItem(Character*);
};