#pragma once

#include "Global.h"

class Item {
private:
	char name[CHARSIZE];	// ������ �̸�
	int money;				// ������ ����
	int count;				// ������ ����
public:
	Item(const char*, int);	// ������ (�̸�, ����)
	~Item();

	// ���� ������
	void SetName(const char*);
	void SetCount(int);
	void SetMoney(int);

	// �б� ������
	const char* GetName();
	int GetCount();
	int GetMoney();

	// �ڽ� ���� �Լ��� �б� ���� ���� ���� �Լ�
	virtual void SetAblity(const char*) = 0;
	virtual const char* GetAblity() = 0;
};