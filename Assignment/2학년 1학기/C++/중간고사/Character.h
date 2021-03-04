#pragma once

#include "Global.h"

class Character {
private:
	char name[CHARSIZE];	// �г���
	char sex[CHARSIZE];		// ����
public:
	Character(const char*);	// ������ (�г���)
	~Character();			// �Ҹ���

	// ���� ������
	void SetName(const char*);
	void SetSex(const char*);
	// �б� ������
	const char* GetName();
	const char* GetSex();

	// �ڽĵ��� �Լ��� �б� ���� ���� ���� �Լ�
	virtual void SetBrood(const char*) = 0;
	virtual void SetJob(const char*) = 0;
	virtual const char* GetBrood() = 0;
	virtual const char* GetJob() = 0;
};