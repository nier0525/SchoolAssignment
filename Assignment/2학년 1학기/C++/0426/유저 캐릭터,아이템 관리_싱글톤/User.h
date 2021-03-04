#pragma once

#include "Global.h"
#include "ListNode.h"
#include "Character.h"
#include "Item.h"

class User {
private:
	char id[CHARSIZE];
	char pass[CHARSIZE];
	bool state;

	int count;
	int money;

	ListNode<Character*>* UserCharList;
	Node<Character*>* SearchChar;

	ListNode<Item*>* UserItemList;
	Node<Item*>* SearchItem;
public:
	User(const char*, const char*);
	~User();

	void SetId(const char*);
	void SetPass(const char*);
	
	Item* Sell(const char*);
	bool Buy(Item*);
	bool InCreseGameMoney(int);
	bool DeCreseGameMoney(int);
	
	bool NewCharacter(Character*);
	bool DelCharacter(Character*);
	bool CharCountCheck();

	const char* GetId();
	const char* GetPass();
	int GetGameMoney();
	int GetCharCount();

	void Login();
	void Logout();

	void SearchCharEnd();
	void SearchItemEnd();

	bool GetCharacter(Character**);
	bool GetItem(Item**);
};