#ifndef _CFoodManager_HCFoodManager
#define _CFoodManager_H
#include "Global.h"

#include "Appliance.h"
#include "Manager.h"

class CApplianceManager:public CManager
{
	CAppliance*  mApplianceList[PRODUCT_MAX_COUNT];	

public:

	CApplianceManager();
	~CApplianceManager();

	void CreateProduct();
	void SearchProduct();
	void DeleteProduct();	
};
#endif