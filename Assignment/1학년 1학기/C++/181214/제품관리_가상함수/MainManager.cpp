#include "MainManager.h"
CMainManager::CMainManager()
{
	for (int i = 0; i < 4; i++) {
		CM[i] = nullptr;
	}
}
CMainManager::~CMainManager()
{
	for (int i = 0; i < 4; i++) {
		if (CM[i] != nullptr) {
			delete CM[i];
		}
	}
}

void CMainManager::Initialize()
{
	CM[FOOD] = new CFoodManager();
	CM[WEAR] = new CWearManager();
	CM[APPLIANCE] = new CApplianceManager();
}

PRODUCT_CLASS CMainManager::GetSelectManager()
{
	int count = CProduct_ID_Info::GetProductArrayCount(PRODUCT_COUNT);

	for(int i=0; i<count; i++)
	{
		printf("%d. %s :\n", i,CProduct_ID_Info::Product_Class[i]);
	}

	printf("����:");
	PRODUCT_CLASS productclass;
	scanf("%d", &productclass);

	return productclass;
}

void CMainManager::Run()
{
	bool endflag=false;	

	while(1)
	{
		printf("<<      ��    ��        >>\n");
		printf("1. ��ǰ �߰�\n");
		printf("2. ��ǰ �˻�\n");
		printf("3. ��ǰ ����\n");	
		printf("4. ����\n");

		printf("����:");
		int sel;
		scanf("%d", &sel);
		
		PRODUCT_CLASS pclass;
		
		switch(sel)
		{
		case PRODUCT_CREATE:
			pclass = GetSelectManager();
			CM[pclass]->CreateProduct();
			break;
			
		case PRODUCT_SEARCH:
			pclass = GetSelectManager();
			CM[pclass]->SearchProduct();
			break;

		case PRODUCT_DELETE:
			pclass = GetSelectManager();
			CM[pclass]->DeleteProduct();
			break;
			
		case PROGRAM_EXIT:
			endflag = true;
			break;		
		}

		if(endflag)
		{
			break;
		}
	}
}
