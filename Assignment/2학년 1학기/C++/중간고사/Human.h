#pragma once

#include "Character.h"

class Human : public Character {	// ĳ���� Ŭ������ ��� ����
private:
	char brood[CHARSIZE];	// ����
	char job[CHARSIZE];		// ����
public:
	Human(const char*);		// ������(���)
	~Human();

	// ���� ������
	void SetBrood(const char*);
	void SetJob(const char*);

	// �б� ������
	const char* GetBrood();
	const char* GetJob();
};