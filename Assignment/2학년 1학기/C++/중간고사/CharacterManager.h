#pragma once

#include "Elf.h"
#include "Human.h"
#include "Oak.h"

#include "User.h"

// ĳ���� ����,������ ����ϴ� �Ŵ��� Ŭ����
// ���� �Ŵ��� Ŭ�������� ���� �������� ���� ���� �޾Ƽ�
// �� ������ ���� ĳ���� ����Ʈ�� ĳ���͸� ���� �Ǵ� �����ϴ� ������ �����
class CharacterManager {
private:
	static const char* brood[];	// ����
	static const char* sex[];	// ����

	static const char* Elfjob[];	// ���� ������
	static const char* Oakjob[];	// ��ũ ������
	static const char* Humanjob[];	// �޸� ������
public:
	CharacterManager();
	~CharacterManager();

	int broodMenu();	// ���� �޴�
	const char* sexMenu();	// ���� �޴�
	const char* jobMenu(int);	// ���� �޴�

	void New(Node<CUser*>*); // ĳ���� ���� �Լ�
	void Del(Node<CUser*>*); // ĳ���� ���� �Լ�
};