#pragma once

#include "ListNode.h"
#include "Character.h"

class User {
private:
	char id[CHARSIZE];
	char pass[CHARSIZE];
	bool state;

	int count;
	int money;

	ListNode<Character*>* UserCharacterList;
	Node<Character*>* SearchCharacter;

public:
	User(const char*, const char*);
	~User();

	void SetId(const char*);
	void SetPass(const char*);
	void SetCount(int);
	void SetMoney(int);

	const char* GetId();
	const char* GetPass();
	int GetCount();
	int GetMoney();

	bool Login();
	bool LogOut();

	bool NewCharacter(Character*);
	bool DelCharacter(Character*);
	bool CharacterCountCheak();

	void SearchEnd();

	bool GetCharacter(Character**);
};