#include "Food.h"

int CFood::ProductNumber = 0;

CFood::CFood(FOOD_CLASS _class, const char* _name, int _price, int _weight):CProduct(_name, _price)
{	
	mFoodClass=_class;	
	mShelf_Life= CProduct_ID_Info::Product_Food_Shelf_Life[_class];
	mWeight=_weight;
	ProductNumber++;
}

CFood::~CFood()
{
}


int CFood::GetShelf_Life()
{ 
	return mShelf_Life; 
}

int CFood::GetWeight()
{ 
	return mWeight; 
}

FOOD_CLASS CFood::GetFoodClass()
{ 
	return mFoodClass; 
}

void CFood::SetShelf_Life(int _life)
{ 
	mShelf_Life = _life;
}

void CFood::MakeId()
{	
	char temp[10];
	strcpy(mId, CProduct_ID_Info::Product_Code[FOOD]);
	strcat(mId, CProduct_ID_Info::Product_Food_Code[mFoodClass]);
		
	itoa(ProductNumber, temp, 10);
	strcat(mId, temp);
}