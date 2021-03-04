#ifndef  _CMAIN_MANAGER_H
#define  _CMAIN_MANAGER_H
#include "Global.h"
#include "Manager.h"
#include "FoodManager.h"
#include "WearManager.h"
#include "ApplianceManager.h"

class CMainManager
{	
	CManager*	CM[4];
public:
	
	CMainManager();
	~CMainManager();

	void Initialize();
	void Run();	
	PRODUCT_CLASS GetSelectManager();
	
};
#endif
