#ifndef PRODUCT_H
#define PRODUCT_H

#include "Global.h"

class cProduct {
private:
	char mProductName[NAMESIZE];
	char mID[10];
	int mCount;
	int mMoney;
public:
	cProduct();
	~cProduct();

	int GetMoney();
	int GetCount();
	const char* GetName();
	const char* GetID();

	bool SetMoney(int);
	bool SetCount(int);
	bool SetName(const char*);

	int Add(int);
	int Delete(int);

	void MakeID();
};

#endif