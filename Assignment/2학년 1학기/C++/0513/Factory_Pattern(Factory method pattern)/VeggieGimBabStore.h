#ifndef _CVEGGIEGIMBAB_STORE_H
#define _CVEGGIEGIMBAB_STORE_H

#include "GimBabStore.h"
#include "VegetableGimBab.h"

class CVegetableGimBabStore:public CGimBabStore
{
private:
	CGimBabStore::Menu*  mVegetableGimbabMenu[10];
	int mCount;

	virtual CGimBab* CreateGimBab(const char*, int);;
	virtual Menu* MenuSelect();;
	virtual CGimBab* WrapGimBab(CGimBab*);

public:
	CVegetableGimBabStore();
	~CVegetableGimBabStore();
};
#endif