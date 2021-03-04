#ifndef  _CMAIN_MANAGER_H
#define  _CMAIN_MANAGER_H
#include "Global.h"
#include "FoodManager.h"
#include "WearManager.h"
#include "ApplianceManager.h"
#include "Product_ID_Info.h"
#include "DataBaseManager.h"

class CMainManager
{	
	CManager**			mMng;	

public:
	
	CMainManager();
	~CMainManager();

	void Initialize();
	void Run();	
	CManager* GetSelectManager();
	
};
#endif
