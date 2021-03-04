#pragma once

#include "Global.h"

class Character;

class Item {
protected:
	char name[CHARSIZE];
	char code[CHARSIZE];
	int kind;
	int price;
public:
	Item(const char*, int);
	virtual ~Item();

	void Setkind(int);

	const char* GetItemName();
	const char* GetItemCode();
	int GetPrice();
	int GetKind();

	virtual void MakeCode() = 0;
	virtual bool OnItem(Character*) = 0;
	virtual bool OffItem(Character*) = 0;

	virtual int GetFirstAbility() = 0;
	virtual int GetSecordAbility() = 0;
};