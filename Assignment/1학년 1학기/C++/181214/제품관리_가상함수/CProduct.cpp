#include "Product.h"

CProduct::CProduct(const char* _pname, int _price)
{
	strcpy(mProductName, _pname);
	mRecving_Date = nullptr;
	mPrice = _price;

}

CProduct::~CProduct()
{
	if (mRecving_Date != nullptr)
	{
		delete mRecving_Date;
	}
}

const char*	CProduct::GetId()
{
	return mId;
}

const char*	CProduct::GetProductName()
{
	return mProductName;
}

const tm* CProduct::GetRecving_Date()
{
	return mRecving_Date; 
}
int			CProduct::GetPrice()
{
	return mPrice;
}
void		CProduct::SetProductName(const char* _pname)
{
	strcpy(mProductName, _pname);	
}

void		CProduct::SetPrice(int _price)
{
	mPrice = _price;
}

void		CProduct::MakeRecving_Date()
{
	if (mRecving_Date == nullptr)
	{
		time_t nowtime;
		time(&nowtime);
		mRecving_Date = new tm();
		memset(mRecving_Date, 0, sizeof(tm));
		tm* temp = localtime(&nowtime);

		memcpy(temp, mRecving_Date, sizeof(tm));
	}
	
}