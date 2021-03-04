#ifndef _CFoodManager_H
#define _CFoodManager_H
#include "Global.h"
#include "Food.h"
#include "Product_ID_Info.h"
#include "Manager.h"
#include "DataBaseManager.h"

class CFoodManager: public CManager
{	

public:

	CFoodManager();
	virtual ~CFoodManager();
	
	virtual void CreateProduct();
	virtual void SearchProduct();
	virtual void DeleteProduct();
};
#endif