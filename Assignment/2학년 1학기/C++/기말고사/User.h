#pragma once

#include "ListNode.h"
#include "Character.h"

// ���� ��ü

class User {
private:
	char id[CHAR];	// ID
	char pass[CHAR];	// PASSWORD
	bool state;			// �α��� ����

	int count;			// ������ �ִ� ĳ���� ����

	// �����ϰ� �ִ� ĳ���� ����Ʈ
	ListNode<Character*>* UserCharacterList;
	// ĳ���� ����Ʈ ��ȸ ���
	Node<Character*>* SearchCharacter;

public:
	User(const char*, const char*);	 // �Ű� ������ ID, PASSWORD
	~User();

	// ���� ������
	void SetId(const char*);
	void SetPass(const char*);
	void SetCount(int);

	// �б� ������
	const char* GetId();
	const char* GetPass();
	int GetCount();

	// �α���,�α׾ƿ��� ���� state �� ����
	bool Login();
	bool LogOut();

	// ĳ���� ����,����, �ִ� ������ �˻縦 ���
	bool NewCharacter(Character*);
	bool DelCharacter(Character*);
	bool CharacterCountCheak();

	// ��ȸ ��� �ʱ�ȭ
	void SearchEnd();
	// ĳ���� ����Ʈ ��ȸ �޼���
	bool GetCharacter(Character**);

	void LoadCharacter(Character* temp) {
		SearchCharacter = UserCharacterList->GetHead()->GetNext();

		while (SearchCharacter) {
			if (!strcmp(SearchCharacter->GetData()->GetNickName(), temp->GetNickName())) {
				return;
			}
			SearchCharacter = SearchCharacter->GetNext();
		}
		UserCharacterList->Insert(temp);
	}
};