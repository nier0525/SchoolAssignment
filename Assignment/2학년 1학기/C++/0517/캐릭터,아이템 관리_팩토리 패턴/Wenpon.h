#pragma once

#include "Item.h"
#define WEAPON_CODE 201

class Character;

class Wenpon : public Item {
private:
	int Att;
	static int Num;
public:
	Wenpon(const char*, int, int);
	~Wenpon();

	virtual int GetFirstAbility();
	virtual int GetSecordAbility();

	virtual void MakeCode();
	virtual bool OnItem(Character*);
	virtual bool OffItem(Character*);
};