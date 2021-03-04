#ifndef _CWEARPRODUCTMANAGER_H
#define _CWEARPRODUCTMANAGER_H
#include "Global.h"

#include "Wear.h"
#include "Manager.h"
#include "DataBaseManager.h"


class CWearManager:public CManager
{

public:
	CWearManager();
	virtual ~CWearManager();	

	virtual void CreateProduct();
	virtual void SearchProduct();
	virtual void DeleteProduct();
	
};
#endif