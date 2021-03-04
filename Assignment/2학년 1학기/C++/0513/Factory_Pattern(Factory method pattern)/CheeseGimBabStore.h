#ifndef _CCHEESEGIMBAB_STORE_H
#define _CCHEESEGIMBAB_STORE_H
#include "GimBabStore.h"
#include "CheeseGimBab.h"

class CCheeseGimBabStore :public CGimBabStore
{
private:
	CGimBabStore::Menu* mCheeseGimbabMenu[10];
	int   mCount;

	virtual CGimBab* CreateGimBab(const char*, int);
	virtual CGimBabStore::Menu* MenuSelect();
	virtual CGimBab* WrapGimBab(CGimBab*);

public:

	CCheeseGimBabStore();
	~CCheeseGimBabStore();
	
};
#endif