#include "FoodManager.h"

CFoodManager::CFoodManager()
{	
}

CFoodManager::~CFoodManager()
{

}


void CFoodManager::CreateProduct()
{
	int count = CProduct_ID_Info::GetProductArrayCount(PRODUCT_FOOD_COUNT);

	for (int i = 0; i < count; i++)
	{
		printf("%d. %s :\n", i, CProduct_ID_Info::Product_Food_Class[i]);
	}

	printf("선택:");
	FOOD_CLASS foodclass;
	scanf("%d", &foodclass);

	printf("이름:");
	char name[PRODUCT_NAME_SIZE];
	scanf("%s", name);

	if (!CheckData(strlen(name), PRODUCT_NAME_SIZE))
	{
		cout << "이름 입력 오류!! " << endl;
		return;
	}

	printf("무게:");
	int weight;
	scanf("%d", &weight);

	if (!CheckData(weight, FOOD_WEIGHT_MAX))
	{
		cout << "무게 입력 오류!! " << endl;
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


	CFood* ptr = new CFood(foodclass, name, price, weight);
	ptr->MakeRecving_Date();
	ptr->MakeId();

	CDataBaseManager::InputData(ptr, FOOD);
}

void CFoodManager::SearchProduct()
{
	printf("검색할 식품 종류를 선택하세요\n");

	int count = CProduct_ID_Info::GetProductArrayCount(PRODUCT_FOOD_COUNT);

	for (int i = 0; i < count; i++)
	{
		printf("%d. %s :\n", i, CProduct_ID_Info::Product_Food_Class[i]);
	}

	printf("선택:");
	FOOD_CLASS foodclass;
	scanf("%d", &foodclass);

	printf("검색상품이름:");
	char name[255];
	scanf("%s", name);

	printf("검색 제품\n");

	CProduct* ptr = CDataBaseManager::SearchData(FOOD);

	if (!ptr)
	{
		printf("검색상품이 존재하지 않습니다.\n");
		CDataBaseManager::SearchEnd();
		return;
	}

	while (ptr)
	{
		CFood* fptr = dynamic_cast<CFood*>(ptr);
		if (fptr)
		{
			if (fptr->GetFoodClass() == foodclass)
			{
				if (!strcmp(fptr->GetProductName(), name))
				{
					printf("제품 아이디:%s 이름:%s  무게:%d  가격:%d\n", fptr->GetId(), fptr->GetProductName(),
						fptr->GetWeight(),fptr->GetPrice());

				}
			}
		}
		else
		{
			break;
		}

		ptr = CDataBaseManager::SearchData(FOOD);
	}
	CDataBaseManager::SearchEnd();
}


void CFoodManager::DeleteProduct()
{
	printf("삭제할 식품 종류를 선택하세요\n");

	int count = CProduct_ID_Info::GetProductArrayCount(PRODUCT_FOOD_COUNT);

	for (int i = 0; i < count; i++)
	{
		printf("%d. %s :\n", i, CProduct_ID_Info::Product_Food_Class[i]);
	}

	printf("선택:");
	FOOD_CLASS foodclass;
	scanf("%d", &foodclass);

	printf("삭제상품이름:");
	char name[PRODUCT_NAME_SIZE];
	scanf("%s", name);

	CProduct* ptr = CDataBaseManager::SearchData(FOOD);

	if (!ptr)
	{
		printf("검색상품이 존재하지 않습니다.\n");
		CDataBaseManager::SearchEnd();
		return;
	}

	while (ptr)
	{
		CFood* fptr = dynamic_cast<CFood*>(ptr);
		if (fptr)
		{
			if (fptr->GetFoodClass() == foodclass)
			{
				if (!strcmp(fptr->GetProductName(), name))
				{
					printf("제품 아이디:%s 이름:%s  무게:%d  가격:%d\n", fptr->GetId(), fptr->GetProductName(), fptr->GetWeight(),
						fptr->GetPrice());
				}
			}
		}
		else
		{
			break;
		}

		ptr = CDataBaseManager::SearchData(FOOD);

	}

	CDataBaseManager::SearchEnd();
		

	printf("삭제상품아이디:");
	char id[255];
	scanf("%s", id);

	ptr = CDataBaseManager::SearchData(FOOD);

	if (!ptr)
	{
		printf("검색상품이 존재하지 않습니다.\n");
		CDataBaseManager::SearchEnd();
		return;
	}

	while (ptr)
	{
		CFood* fptr = dynamic_cast<CFood*>(ptr);
		if (fptr)
		{

			if (!strcmp(fptr->GetId(), id))
			{
				if (CDataBaseManager::DeleteData(ptr, FOOD))
				{
					printf("선택상품이 삭제되었습니다 \n");
					break;
				}
			}
		}
		else
		{
			break;
		}


		ptr = CDataBaseManager::SearchData(FOOD);
	}

	CDataBaseManager::SearchEnd();
}

