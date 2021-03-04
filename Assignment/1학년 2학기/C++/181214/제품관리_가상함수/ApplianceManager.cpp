#include "ApplianceManager.h"

CApplianceManager::CApplianceManager()
{
	memset(mApplianceList, 0, sizeof(mApplianceList));
}

CApplianceManager::~CApplianceManager()
{
	for (int i = 0; i < mProduct_Count; i++)
	{
		delete mApplianceList[i];
	}

}

void CApplianceManager::CreateProduct()
{	
	int count = CProduct_ID_Info::GetProductArrayCount(PRODUCT_APPLIANCE_COUNT);
	
	for(int i=0; i<count; i++)
	{
		printf("%d. %s :\n", i,CProduct_ID_Info::Product_Appliance_Class[i]);
	}

	printf("선택:");
	APPLIANCE_CLASS applianceclass;
	scanf("%d", &applianceclass);

	printf("이름:");
	char name[255];
	scanf("%s", name);

	if (!CheckData(strlen(name), PRODUCT_NAME_SIZE))
	{
		cout << "이름 입력 오류!! " << endl;
		return;
	}

	printf("모델명:");
	char model[255];
	scanf("%s", model);	

	if (!CheckData(strlen(model), APPLIANCE_MODEL_NUMBER_SIZE))
	{
		cout << "모델명 입력 오류!! " << endl;
		return;
	}

	printf("가격:");
	int price;
	scanf("%d", &price);

	if (!CheckData(price, PRICE_LEMIT))
	{
		cout << "가격 입력 오류!! " << endl;
		return;
	}


	CAppliance* ptr = new CAppliance(applianceclass, name, model, price);
	ptr->MakeRecving_Date();
	ptr->MakeId();
	
	mApplianceList[mProduct_Count++] = ptr;
}

void CApplianceManager::SearchProduct()
{
	printf("검색할 가전제품 종류를 선택하세요\n");

	int count = CProduct_ID_Info::GetProductArrayCount(PRODUCT_APPLIANCE_COUNT);
	
	for(int i=0; i<count; i++)
	{
		printf("%d. %s :\n", i,CProduct_ID_Info::Product_Appliance_Class[i]);
	}

	printf("선택:");
	APPLIANCE_CLASS applianceclass;
	scanf("%d", &applianceclass);

	printf("검색상품이름:");
	char name[255];
	scanf("%s", name);

	printf("검색 제품 리스트\n");

	for (int i = 0; i < mProduct_Count; i++)
	{
		if (mApplianceList[i]->GetApplianceClass() == applianceclass)
		{
			if (!strcmp(mApplianceList[i]->GetProductName(), name))
			{
				printf("이름:%s  모델명:%s  가격:%d\n", mApplianceList[i]->GetProductName(), mApplianceList[i]->GetModelNum(), mApplianceList[i]->GetPrice());
			}
		}
	}	
}

void CApplianceManager::DeleteProduct()
{
	printf("삭제할 가전제품 종류를 선택하세요\n");

	int count = CProduct_ID_Info::GetProductArrayCount(PRODUCT_APPLIANCE_COUNT);
	
	for(int i=0; i<count; i++)
	{
		printf("%d. %s :\n", i,CProduct_ID_Info::Product_Appliance_Class[i]);
	}

	printf("선택:");
	APPLIANCE_CLASS applianceclass;
	scanf("%d", &applianceclass);

	printf("삭제상품이름:");
	char name[255];
	scanf("%s", name);	

	bool flag = false;

	for (int i = 0; i < mProduct_Count; i++)
	{
		if (mApplianceList[i]->GetApplianceClass() == applianceclass)
		{
			if (!strcmp(mApplianceList[i]->GetProductName(), name))
			{
				printf("이름:%s  모델명:%s  id:%s 가격:%d\n", mApplianceList[i]->GetProductName(), mApplianceList[i]->GetModelNum(), mApplianceList[i]->GetId(), mApplianceList[i]->GetPrice());
			}
		}
	}

	if (!flag)
	{
		printf("해당 제품이 없습니다.\n");
		return;
	}

	printf("삭제상품아이디:");
	char id[255];
	scanf("%s", id);

	flag = false;

	for (int i = 0; i < mProduct_Count; i++)
	{
		if (mApplianceList[i]->GetApplianceClass() == applianceclass)
		{
			if (!strcmp(mApplianceList[i]->GetId(), id))
			{
				delete mApplianceList[i];
				for (int j = i; j < mProduct_Count - 1; j++)
				{
					mApplianceList[j] = mApplianceList[j + 1];
				}

				mApplianceList[mProduct_Count - 1] = nullptr;
				mProduct_Count--;
				flag = true;
				break;
			}
		}
	}

	if (!flag)
	{
		printf("해당 제품이 없습니다.\n");		
	}
}

