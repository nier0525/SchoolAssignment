#include "WearManager.h"

CWearManager::CWearManager()
{
	memset(mWearList, 0, sizeof(mWearList));
}

CWearManager::~CWearManager()
{
	for (int i = 0; i < mProduct_Count; i++)
	{
		delete mWearList[i];
	}

}

void CWearManager::CreateProduct()
{
	int count = CProduct_ID_Info::GetProductArrayCount(PRODUCT_WEAR_COUNT);

	for (int i = 0; i < count; i++)
	{
		printf("%d. %s :\n", i, CProduct_ID_Info::Product_Wear_Class[i]);
	}

	printf("선택:");
	WEAR_CLASS wearclass;
	scanf("%d", &wearclass);

	printf("이름:");
	char name[255];
	scanf("%s", name);

	if (!CheckData(strlen(name), PRODUCT_NAME_SIZE))
	{
		cout << "이름 입력 오류!! " << endl;
		return;
	}

	printf("사이즈:");
	int size;
	scanf("%d", &size);

	printf("가격:");
	int price;
	scanf("%d", &price);

	if (!CheckData(price, PRICE_LEMIT))
	{
		cout << "가격 입력 오류!! " << endl;
		return;
	}


	CWear* ptr = new CWear(wearclass, name, price, size);
	ptr->MakeRecving_Date();
	ptr->MakeId();


	mWearList[mProduct_Count++] = ptr;
}

void CWearManager::SearchProduct()
{
	printf("검색할 의류제품 종류를 선택하세요\n");

	int count = CProduct_ID_Info::GetProductArrayCount(PRODUCT_WEAR_COUNT);

	for (int i = 0; i < count; i++)
	{
		printf("%d. %s :\n", i, CProduct_ID_Info::Product_Wear_Class[i]);
	}

	printf("선택:");
	WEAR_CLASS wearclass;
	scanf("%d", &wearclass);

	printf("검색상품이름:");
	char name[255];
	scanf("%s", name);

	printf("검색 제품 리스트\n");

	for (int i = 0; i < mProduct_Count; i++)
	{
		if (mWearList[i]->GetWearClass() == wearclass)
		{
			if (!strcmp(mWearList[i]->GetProductName(), name))
			{
				printf("이름:%s  사이즈:%d  가격:%d\n", mWearList[i]->GetProductName(), mWearList[i]->GetSize(), mWearList[i]->GetPrice());
			}
		}
	}
}

void CWearManager::DeleteProduct()
{
	printf("삭제할 의류제품 종류를 선택하세요\n");

	int count = CProduct_ID_Info::GetProductArrayCount(PRODUCT_WEAR_COUNT);

	for (int i = 0; i < count; i++)
	{
		printf("%d. %s :\n", i, CProduct_ID_Info::Product_Wear_Class[i]);
	}

	printf("선택:");
	WEAR_CLASS wearclass;
	scanf("%d", &wearclass);

	printf("삭제상품이름:");
	char name[255];
	scanf("%s", name);

	bool flag = false;

	for (int i = 0; i < mProduct_Count; i++)
	{
		if (mWearList[i]->GetWearClass() == wearclass)
		{
			if (!strcmp(mWearList[i]->GetProductName(), name))
			{
				printf("이름:%s  id:%s  사이즈:%d 가격:%d\n", mWearList[i]->GetProductName(), mWearList[i]->GetId(), mWearList[i]->GetSize(), mWearList[i]->GetPrice());
				flag = true;
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
		if (mWearList[i]->GetWearClass() == wearclass)
		{
			if (!strcmp(mWearList[i]->GetId(), id))
			{
				delete mWearList[i];
				for (int j = i; j < mProduct_Count - 1; j++)
				{
					mWearList[j] = mWearList[j + 1];
				}

				mWearList[mProduct_Count - 1] = nullptr;
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

