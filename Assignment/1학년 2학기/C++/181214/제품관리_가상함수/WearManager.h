#ifndef _CWEARPRODUCTMANAGER_H
#define _CWEARPRODUCTMANAGER_H
#include "Global.h"

#include "Wear.h"
#include "Manager.h"



class CWearManager:public CManager
{
	CWear*  mWearList[PRODUCT_MAX_COUNT];

public:
	CWearManager();
	~CWearManager();
	
	void CreateProduct();
	void SearchProduct();
	void DeleteProduct();
	
};
#endif