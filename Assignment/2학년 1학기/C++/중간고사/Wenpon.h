#pragma once

#include "Item.h"

class Wenpon : public Item {	// ������ Ŭ���� ���
private:
	char abilty[CHARSIZE]; // ������ ����
public:
	Wenpon(const char*, int);	// ������ (���)
	~Wenpon();

	// ���� ������
	void SetAblity(const char*);

	// �б� ������
	const char* GetAblity();
};