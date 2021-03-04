#ifndef _CFoodManager_HCFoodManager
#define _CFoodManager_H
#include "Global.h"

#include "Appliance.h"
#include "Manager.h"
#include "DataBaseManager.h"

class CApplianceManager:public CManager
{
	
public:

	CApplianceManager();
	virtual ~CApplianceManager();	

	virtual void CreateProduct();
	virtual void SearchProduct();
	virtual void DeleteProduct();
};
#endif