#pragma once

#include "Item.h"

class Portion : public Item {	// ������ Ŭ������ ���
private:
	char abilty[CHARSIZE];	// ������ ����
public:
	Portion(const char*, int);	// ������ (���)
	~Portion();

	// ���� ������
	void SetAblity(const char*);
	// �б� ������
	const char* GetAblity();
};