#ifndef ITEM_H
#define ITEM_H
#include "Base.h"

class cItem {
private:
	char mItemName[NAMESIZE];
	char mId[IDSIZE];
	int mItemid;
	int mFreeAs;
	int mMoney;
	int mCount;

public:
	cItem();
	~cItem();

	const char* GetId();
	const char* GetItemName();
	int GetItemid();
	int GetFreeAs();
	int GetMoney();
	int GetCount();

	bool SetItemName(const char*);
	bool SetItemid(int);
	bool SetFreeAs(int);
	bool SetMoney(int);
	bool SetCount(int);

	int InCount(int);
	int DeCount(int);

};

#endif