#ifndef _CCHEESEGIMBAB_H
#define _CCHEESEGIMBAB_H
#include "Global.h"
#include "GimBab.h"

class CCheeseGimBab :public CGimBab
{
public :
	CCheeseGimBab(const char* _name, int _price);
	~CCheeseGimBab();

	virtual void Prepare();
	virtual void IngredientSet();
	virtual void Roll();
	virtual void Cut();

};
#endif