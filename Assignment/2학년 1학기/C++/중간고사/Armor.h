#pragma once

#include "Item.h"

class Armor : public Item {	// ������ Ŭ������ ���
private:
	char abilty[CHARSIZE];	// ������ ����
public:
	Armor(const char*, int);	// ������(���)
	~Armor();

	// ���� ������
	void SetAblity(const char*);
	// �б� ������
	const char* GetAblity();
};