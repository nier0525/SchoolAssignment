#ifndef _CPRODUCT_ID_INFO_H
#define _CPRODUCT_ID_INFO_H
#include "Global.h"

class CProduct_ID_Info
{
public:	

	static const char* Product_Class[];
	static const char* Product_Code[];

	static const char* Product_Food_Class[];
	static const char* Product_Food_Code[];
	static int	 Product_Food_Shelf_Life[];

	static const char* Product_Wear_Class[];
	static const char* Product_Wear_Code[];
		
	static const char* Product_Appliance_Class[];
	static const char* Product_Appliance_Code[];
	
	static int GetProductArrayCount(COUNT_CLASS _class);
};

#endif