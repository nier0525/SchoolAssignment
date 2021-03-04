#include "WearManager.h"

CWearManager::CWearManager()
{

}

CWearManager::~CWearManager()
{
	
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

	CDataBaseManager::InputData(ptr, WEAR);
	
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

	CProduct* ptr = CDataBaseManager::SearchData(WEAR);
	if (!ptr)
	{
		printf("검색상품이 존재하지 않습니다.\n");
		CDataBaseManager::SearchEnd();
		return;
	}

	while (ptr)
	{
		CWear* fptr = dynamic_cast<CWear*>(ptr);
		if (fptr)
		{
			if (fptr->GetWearClass() == wearclass)
			{
				if (!strcmp(fptr->GetProductName(), name))
				{
					printf("이름:%s  싸이즈:%d  가격:%d\n", fptr->GetProductName(), fptr->GetSize(), fptr->GetPrice());
				}
			}
		}
		ptr = CDataBaseManager::SearchData(WEAR);
	}

	CDataBaseManager::SearchEnd();
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

	CProduct* ptr = CDataBaseManager::SearchData(WEAR);

	if (!ptr)
	{
		printf("검색상품이 존재하지 않습니다.\n");
		CDataBaseManager::SearchEnd();
		return;
	}

	while (ptr)
	{
		CWear* fptr = dynamic_cast<CWear*>(ptr);
		if (fptr)
		{
			if (fptr->GetWearClass() == wearclass)
			{
				if (!strcmp(fptr->GetProductName(), name))
				{
					printf("제품 아이디:%s 이름:%s  싸이즈:%d  가격:%d\n", fptr->GetId(), fptr->GetProductName(), 
						fptr->GetSize(),  fptr->GetPrice());

				}
			}
		}
		ptr = CDataBaseManager::SearchData(WEAR);

	}

	CDataBaseManager::SearchEnd();

	printf("삭제상품아이디:");
	char id[255];
	scanf("%s", id);

	ptr = CDataBaseManager::SearchData(WEAR);

	if (!ptr)
	{
		printf("검색상품이 존재하지 않습니다.\n");
		CDataBaseManager::SearchEnd();
		return;
	}

	while (ptr)
	{
		CWear* fptr = dynamic_cast<CWear*>(ptr);
		if (!strcmp(fptr->GetId(), id))
		{
			if (CDataBaseManager::DeleteData(ptr, WEAR))
			{
				printf("선택상품이 삭제되었습니다 \n");
				break;
			}
		}

		ptr = CDataBaseManager::SearchData(FOOD);
	}

	CDataBaseManager::SearchEnd();
}

