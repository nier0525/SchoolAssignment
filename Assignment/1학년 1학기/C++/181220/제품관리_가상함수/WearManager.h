#ifndef _CWEARPRODUCTMANAGER_H
#define _CWEARPRODUCTMANAGER_H
#include "Global.h"

#include "Wear.h"
#include "Manager.h"
#include "ListNode.h"
#include "Node.h"

class CWearManager:public CManager
{
	ListNode<CWear>*  mWearList;
public:
	CWearManager();
	~CWearManager();
	
	void CreateProduct();
	void SearchProduct();
	void DeleteProduct();
	
};
#endif