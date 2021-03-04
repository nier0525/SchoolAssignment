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

	printf("선택:");
	PRODUCT_CLASS productclass;
	scanf("%d", &productclass);

	return productclass;
}

void CMainManager::Run()
{
	bool endflag=false;	

	while(1)
	{
		printf("<<      선    택        >>\n");
		printf("1. 상품 추가\n");
		printf("2. 상품 검색\n");
		printf("3. 상품 삭제\n");	
		printf("4. 종료\n");

		printf("선택:");
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
