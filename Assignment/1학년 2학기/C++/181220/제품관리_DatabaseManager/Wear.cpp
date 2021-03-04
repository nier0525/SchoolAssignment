#include "Wear.h"
int  CWear::ProductNumber = 0;

CWear::CWear(WEAR_CLASS _class, const char* _name, int _price, int _size):CProduct(_name, _price)
{
	mWearClass=_class;
	mSize=_size;
	ProductNumber++;
}

CWear::~CWear()
{
}

void CWear::MakeId()
{
	char temp[10];

	strcpy(mId, CProduct_ID_Info::Product_Code[WEAR]);
	strcat(mId, CProduct_ID_Info::Product_Wear_Code[mWearClass]);

	itoa(ProductNumber, temp, 10);
	strcat(mId, temp);
}

int	 CWear::GetSize() 
{ 
	return mSize; 
}
WEAR_CLASS CWear::GetWearClass()
{ 
	return mWearClass; 
}

void CWear::SetSize(int _size)
{ 
	mSize = _size; 
}
void CWear::SetWearClass(WEAR_CLASS _wclass)
{ 
	mWearClass = _wclass;
}