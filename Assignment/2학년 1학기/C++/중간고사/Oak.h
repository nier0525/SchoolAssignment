#pragma once

#include "Character.h"

class Oak : public Character {	// ĳ���� Ŭ������ ��� ����
private:
	char brood[CHARSIZE];	// ����
	char job[CHARSIZE];		// ����
public:
	Oak(const char*);		// ������(���)
	~Oak();

	// ���� ������
	void SetBrood(const char*);
	void SetJob(const char*);

	// �б� ������
	const char* GetBrood();
	const char* GetJob();
};