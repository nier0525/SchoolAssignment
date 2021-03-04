#ifndef  CMANAGER_H
#define  CMANAGER_H

#include "Global.h"
#include "Product.h"

class CManager
{
protected:

	int					mProduct_Count;

public:

	CManager();
	~CManager();

	int GetProduct_Count();
	bool CheckData(int,int);

	virtual void CreateProduct() = 0;
	virtual void SearchProduct() = 0;
	virtual void DeleteProduct() = 0;

};
#endif