#include "Product_ID_Info.h"
const char* CProduct_ID_Info::Product_Class[]={"식품", "의류", "가전"};
const char* CProduct_ID_Info::Product_Code[]={"100", "200","300"};


const char* CProduct_ID_Info::Product_Food_Class[]={"야채","과일","육류","곡물"};
const char* CProduct_ID_Info::Product_Food_Code[]={"11","22", "33","44"};
int		 CProduct_ID_Info::Product_Food_Shelf_Life[]={5, 14, 20, 50};

const char* CProduct_ID_Info::Product_Wear_Class[]={"남성의류","여성의류"};
const char* CProduct_ID_Info::Product_Wear_Code[]={"11","22"};

const char* CProduct_ID_Info::Product_Appliance_Class[]={"TV","냉장고","세탁기"};
const char* CProduct_ID_Info::Product_Appliance_Code[]={"11","22","33"};

int CProduct_ID_Info::GetProductArrayCount(COUNT_CLASS _class)
{
	switch(_class)
	{
	case PRODUCT_COUNT:
		return sizeof(Product_Class)/sizeof(char*);		
	case PRODUCT_FOOD_COUNT:
		return sizeof(Product_Food_Class)/sizeof(char*);		
	case PRODUCT_WEAR_COUNT:
		return sizeof(Product_Wear_Class)/sizeof(char*);		
	case PRODUCT_APPLIANCE_COUNT:
		return sizeof(Product_Appliance_Class)/sizeof(char*);
	}

	return -1;
}