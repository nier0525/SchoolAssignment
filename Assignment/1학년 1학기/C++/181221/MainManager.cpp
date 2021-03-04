#include "MainManager.h"
CMainManager::CMainManager()
{
	mMng = nullptr;
	
}
CMainManager::~CMainManager()
{
	if (mMng != nullptr)
	{
		for (int i = 0; i < CProduct_ID_Info::GetProductArrayCount(PRODUCT_COUNT); i++)
		{
			if (mMng[i] != nullptr)
			{
				delete mMng[i];
			}
		}

		delete[] mMng;
	}	

	CDataBaseManager::End();
}

void CMainManager::Initialize()
{
	mMng = new CManager*[CProduct_ID_Info::GetProductArrayCount(PRODUCT_COUNT)];

	mMng[FOOD] = new CFoodManager();
	mMng[WEAR] = new CWearManager();
	mMng[APPLIANCE] = new CApplianceManager();

	CDataBaseManager::Initialize();
}

CManager* CMainManager::GetSelectManager()
{
	int count = CProduct_ID_Info::GetProductArrayCount(PRODUCT_COUNT);

	for(int i=0; i<count; i++)
	{
		printf("%d. %s :\n", i,CProduct_ID_Info::Product_Class[i]);
	}

	printf("선택:");
	PRODUCT_CLASS productclass;
	scanf("%d", &productclass);

	return mMng[productclass];
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
		
		CManager* mng;
		
		switch(sel)
		{
		case PRODUCT_CREATE:
			mng = GetSelectManager();
			mng->CreateProduct();			
			break;
		case PRODUCT_SEARCH:
			mng = GetSelectManager();
			mng->SearchProduct();
			
			break;
		case PRODUCT_DELETE:
			mng = GetSelectManager();
			mng->DeleteProduct();			
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
