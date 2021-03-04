#ifndef _CFoodManager_H
#define _CFoodManager_H
#include "Global.h"
#include "Food.h"
#include "Product_ID_Info.h"
#include "Manager.h"

class CFoodManager: public CManager
{	
	CFood*		mFoodList[PRODUCT_MAX_COUNT];	

public:

	CFoodManager();
	~CFoodManager();
	
	void CreateProduct();
	void SearchProduct();
	void DeleteProduct();	
};
#endif