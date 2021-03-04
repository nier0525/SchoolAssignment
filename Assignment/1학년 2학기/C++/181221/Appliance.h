#ifndef _CAPPLIANCE_H
#define _CAPPLIANCE_H
#include "Global.h"
#include "Product.h"
#include "Product_ID_Info.h"

class CAppliance:public CProduct
{
	char				mModelNum[APPLIANCE_MODEL_NUMBER_SIZE];
	APPLIANCE_CLASS		mApplianceClass;
	static int			ProductNumber;

public :
	CAppliance(APPLIANCE_CLASS , const char* , const char*,  int);
	~CAppliance();

	const char* GetModelNum();
	int  	    GetApplianceClass();
	int			GetProductNumber();

	void SetModelNum(const char*);
	void SetApplianceClass(APPLIANCE_CLASS);
	

	virtual void MakeId();
};

#endif