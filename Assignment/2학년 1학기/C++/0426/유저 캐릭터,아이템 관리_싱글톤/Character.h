#pragma once

#include "Global.h"
#include "ListNode.h";
#include "Item.h";

class Character {
private:
	char Name[CHARSIZE];
	int BroodCode;
	int Level;
	int HP;
	int MP;
	int Att;
	int Def;

	ListNode<Item*>* CharItemList;
	Node<Item*>* SearchItem;
public:
	Character(const char*);
	~Character();

	const char* GetCharName();
	int GetBroodCode();
	int GetLevel();
	int GetHP();
	int GetMP();
	int GetAtt();
	int GetDef();

	void SetCharName(const char*);
	void SetBroodCode(int);
	void LevelUp();
	void SetHP(int);
	void SetMP(int);
	void SetAtt(int);
	void SetDef(int);

	bool OnItem(Item*);
	bool OffItem(Item*);

	bool GetItem(Item**);
	void SearchItemEnd();
};