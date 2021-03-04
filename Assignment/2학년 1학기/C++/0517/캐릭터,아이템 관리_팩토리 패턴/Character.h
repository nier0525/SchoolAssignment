#pragma once

#include "ListNode.h"
#include "Item.h"

class Character {
private:
	char Name[CHARSIZE];
	int JobCode;
	int BroodCode;
	int Level;
	int Hp;
	int Mp;
	int Att;
	int Def;

	int money;

	ListNode<Item*>* Inven;
	Node<Item*>* SearchItem;
public:
	Character(const char*);
	~Character();

	const char* GetNickName();
	int GetBroodCode();
	int GetJobCode();
	int GetLevel();
	int GetHp();
	int GetMp();
	int GetAtt();
	int GetDef();
	int GetMoney();

	void SetNickName(const char*);
	void SetBroodCode(int);
	void SetJobCode(int);
	void LevelUp();
	void SetHp(int);
	void SetMp(int);
	void SetAtt(int);
	void SetDef(int);
	void SetMoney(int);

	Item* Sell(const char*);
	bool Buy(Item*);
	bool InCreseGameMoney(int);
	bool DeCreseGameMoney(int);

	void SearchEnd();
	bool GetItem(Item**);
};