#ifndef _CGIMBAB_H
#define _CGIMBAB_H
#include "Global.h"

class CGimBab
{
private:

	char mName[255];
	int  mPrice;

public:
	CGimBab(const char*, int);
	virtual ~CGimBab();

	const char* GetName();
	void SetName(const char*);

	int GetPrice();
	void SetPrice(int _money);

	virtual void Prepare()=0;
	virtual void IngredientSet()=0;
	virtual void Roll()=0;
	virtual void Cut()=0;

};
#endif