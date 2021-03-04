#pragma once

#include "Item.h"
#define ACCESSORIES_CODE 203

class Character;

class Accessories : public Item {
private:
	int HP;
	int MP;
	static int Num;
public:
	Accessories(const char*, int, int, int);
	~Accessories();

	virtual int GetFirstAbility();
	virtual int GetSecordAbility();

	virtual void MakeCode();
	virtual bool OnItem(Character*);
	virtual bool OffItem(Character*);
};