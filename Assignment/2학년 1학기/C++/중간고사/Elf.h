#pragma once

#include "Character.h"

class Elf : public Character {	// ĳ���� Ŭ������ ��� ����
private:
	char brood[CHARSIZE]; // ����
	char job[CHARSIZE];		// ����
public:
	Elf(const char*);	// ������(���)
	~Elf();

	// ���� ������
	void SetBrood(const char*);
	void SetJob(const char*);

	// �б� ������
	const char* GetBrood();
	const char* GetJob();
};