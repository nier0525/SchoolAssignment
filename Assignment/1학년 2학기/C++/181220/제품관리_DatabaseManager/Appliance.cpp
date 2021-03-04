#include "Appliance.h"

int CAppliance::ProductNumber = 0;

CAppliance::CAppliance(APPLIANCE_CLASS _class, const char* _name, const char* _modelnum, int _price) :CProduct( _name,  _price)
{
	mApplianceClass=_class;
	strcpy(mModelNum, _modelnum);	
	ProductNumber++;
}


CAppliance::~CAppliance()
{	
}

void CAppliance::SetModelNum(const char* _modelnum)
{	
	strcpy(mModelNum, _modelnum);
}

void CAppliance::MakeId()
{
	char temp[10];
	
	strcpy(mId, CProduct_ID_Info::Product_Code[APPLIANCE]);
	strcat(mId, CProduct_ID_Info::Product_Appliance_Code[mApplianceClass]);

	itoa(ProductNumber, temp, 10);
	strcat(mId, temp);
}

const char* CAppliance::GetModelNum()
{ 
	return mModelNum; 
}
int CAppliance::GetApplianceClass()
{ 
	return mApplianceClass; 
}

void CAppliance::SetApplianceClass(APPLIANCE_CLASS _class)
{ 
	mApplianceClass = _class; 
}

int			CAppliance::GetProductNumber()
{
	return ProductNumber;
}