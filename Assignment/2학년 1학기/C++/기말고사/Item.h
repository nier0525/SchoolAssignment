#pragma once

#include "Global.h"

// ������ Ŭ����

class Character;

class Item {
protected:
	char name[CHAR];	// ������ �̸�
	char code[CHAR];	// ������ �ڵ�
	int price;			// ������ ����
public:
	Item(const char*, int);	// �Ű� ������ �̸��� ����
	virtual ~Item();

	// �б� ������
	const char* GetItemName();
	const char* GetItemCode();
	int GetPrice();

	// ���� ���� �Լ�
	virtual void MakeCode() = 0;
	virtual bool OnItem(Character*) = 0;
	virtual bool OffItem(Character*) = 0;

	virtual int GetFirstAbility() = 0;
	virtual int GetSecordAbility() = 0;
};