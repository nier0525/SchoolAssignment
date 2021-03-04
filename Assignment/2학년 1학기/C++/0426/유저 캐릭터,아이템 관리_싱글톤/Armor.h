#pragma once

#include "Item.h"
#define ARMOR_CODE 202

class Character;

class Armor : public Item {
private:
	int Def;
	static int Num;
public:
	Armor(const char*, int, int);
	~Armor();

	virtual int GetFirstAbility();
	virtual int GetSecordAbility();

	virtual void MakeCode();
	virtual bool OnItem(Character*);
	virtual bool OffItem(Character*);
};