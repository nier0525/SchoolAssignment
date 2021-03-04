#ifndef  _CDATA_MANAGER_H
#define  _CDATA_MANAGER_H
#include "Global.h"
#include "LinkedList.h"
#include "Product.h"

class CDataBaseManager
{
	
	static CLinkedList<CProduct*>*			mFoodLst;
	static CLinkedList<CProduct*>*			mWearLst;
	static CLinkedList<CProduct*>*			mApplianceLst;
	static CNode<CProduct*>*				mSearchNode;
	

public:
	CDataBaseManager();
	~CDataBaseManager();
	
	static void Initialize();
	static void End();

	static void SearchEnd();
	static bool InputData(CProduct*, PRODUCT_CLASS);
	static CProduct* SearchData(PRODUCT_CLASS);
	static bool DeleteData(CProduct*, PRODUCT_CLASS);

	static bool Login(PRODUCT_CLASS);
	static bool Logout(PRODUCT_CLASS);
	static bool New(CProduct*, PRODUCT_CLASS);
};

#endif