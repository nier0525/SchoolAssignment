#ifndef _CSTORE_H
#define _CSTORE_H
#include "Global.h"
#include "GimBab.h"



class CGimBabStore
{
protected:
	struct Menu
	{
		char gimbab_name[255];
		int  price;
		
		Menu(const char* _name, int _price)
		{
			strcpy(gimbab_name, _name);
			price = _price;
		}

	};

	virtual CGimBab* CreateGimBab(const char*, int) = 0;
	virtual Menu* MenuSelect() = 0;
	virtual CGimBab* WrapGimBab(CGimBab*) = 0;

public:
	CGimBabStore();
	virtual ~CGimBabStore();
	CGimBab* OrderGimBab();
};
#endif