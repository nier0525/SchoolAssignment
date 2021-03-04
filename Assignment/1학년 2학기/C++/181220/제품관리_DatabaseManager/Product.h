#ifndef CPRODUCT_H
#define CPRODUCT_H
#include "Global.h"

class CProduct
{
private:
	char		mProductName[PRODUCT_NAME_SIZE];
	tm*			mRecving_Date;
	int			mPrice;

protected:
	char		mId[PRODUCT_ID_SIZE];

public:
	CProduct(const char*, int);
	virtual ~CProduct();

	const char*	GetId();
	const char*	GetProductName();
	const tm*	GetRecving_Date();
	int			GetPrice();	

	void		SetProductName(const char*);
	void		SetPrice(int);
	
	void		MakeRecving_Date();
	
	virtual void MakeId() = 0;

};
#endif
