#ifndef  CMANAGER_H
#define  CMANAGER_H

#include "Global.h"
#include "Product.h"

class CManager
{

public:

	CManager();
	virtual ~CManager();

	
	bool CheckData(int,int);

	virtual void CreateProduct() = 0;
	virtual void SearchProduct() = 0;
	virtual void DeleteProduct() = 0;
 
};
#endif