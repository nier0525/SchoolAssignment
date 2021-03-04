#ifndef FOOD_H
#define FOOD_H
#include "Base.h"

class cFood {
private:
	char mFoodName[NAMESIZE];
	char mId[IDSIZE];
	int mRank;
	int mGram;
	int mMoney;
	int mCount;

public:
	cFood();
	~cFood();

	const char* GetId();
	const char* GetFoodName();
	int GetRank();
	int GetGram();
	int GetMoney();
	int GetCount();

	bool SetFoodName(const char*);
	bool SetRand(int);
	bool SetGram(int);
	bool SetMoney(int);
	bool SetCount(int);

	int InCount(int);
	int DeCount(int);

};

#endif