#ifndef _CWEAR_H
#define _CWEAR_H

#include "Product.h"
#include "Product_ID_Info.h"

class CWear: public CProduct
{	
	int			mSize;	
	WEAR_CLASS   mWearClass;
	static int   ProductNumber;

public:	
	CWear(WEAR_CLASS, const char*, int, int);
	~CWear();
	int	 GetSize();
	WEAR_CLASS GetWearClass();

	void SetSize(int _size);
	void SetWearClass(WEAR_CLASS);

	virtual void MakeId();
};


#endif