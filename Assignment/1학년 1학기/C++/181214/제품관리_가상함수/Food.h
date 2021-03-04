#ifndef _CFOOD_H
#define _CFOOD_H
#include "Global.h"
#include "Product.h"
#include "Product_ID_Info.h"

class CFood:public CProduct
{
	FOOD_CLASS	mFoodClass;
	int			mShelf_Life;	
	int			mWeight;
	static int  ProductNumber;

public:	
	CFood(FOOD_CLASS, const char* , int, int);
	~CFood();

	int GetShelf_Life();
	int GetWeight();
	FOOD_CLASS GetFoodClass();

	void SetShelf_Life(int);

	void MakeId();
};
#endif