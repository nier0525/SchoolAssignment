#ifndef WEAR_H
#define WEAR_H
#include "Base.h"

class cWear {
private:
	char mWearName[NAMESIZE];
	char mId[IDSIZE];
	int mSize;
	char mSex[SEXSIZE];
	int mMoney;
	int mCount;

public:
	cWear();
	~cWear();

	const char* GetId();
	const char* GetWearName();
	
	const char* GetSex();
	int GetSize();
	
	int GetMoney();
	int GetCount();

	bool SetWearName(const char*);
	
	bool SetSex(const char*);
	bool SetSize(int);
	
	bool SetMoney(int);
	bool SetCount(int);

	int InCount(int);
	int DeCount(int);

};

#endif#pragma once
