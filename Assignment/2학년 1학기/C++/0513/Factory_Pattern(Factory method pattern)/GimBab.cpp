#include "GimBab.h"

CGimBab::CGimBab(const char* _name, int _price)
{
	strcpy(mName, _name);	
	mPrice = _price;
}

CGimBab::~CGimBab()
{
	
}

void CGimBab::SetName(const char* _name)
{	
	strcpy(mName, _name);
}


const char* CGimBab::GetName()
{ 
	return mName; 
}

int CGimBab::GetPrice()
{ 
	return mPrice;
}

void CGimBab::SetPrice(int _money)
{
	mPrice = _money; 
}

