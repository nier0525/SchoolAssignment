#include "FoodManager.h"

CFoodManager::CFoodManager()
{
	memset(mFoodList, 0, sizeof(mFoodList));
}

CFoodManager::~CFoodManager()
{
	for (int i = 0; i < mProduct_Count; i++)
	{
		delete mFoodList[i];
	}

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

	mFoodList[mProduct_Count++] = ptr;
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

	for (int i = 0; i < mProduct_Count; i++)
	{
		if (mFoodList[i]->GetFoodClass() == foodclass)
		{
			if (!strcmp(mFoodList[i]->GetProductName(), name))
			{
				printf("이름:%s  가격:%d 무게:%d\n", mFoodList[i]->GetProductName(),  mFoodList[i]->GetPrice(), mFoodList[i]->GetWeight());
			}
		}
	}
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

	bool flag = false;

	for (int i = 0; i < mProduct_Count; i++)
	{
		if (mFoodList[i]->GetFoodClass() == foodclass)
		{
			if (!strcmp(mFoodList[i]->GetProductName(), name))
			{
				printf("이름:%s  id:%s 가격:%d\n", mFoodList[i]->GetProductName(),  mFoodList[i]->GetId(), mFoodList[i]->GetPrice());
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
		if (mFoodList[i]->GetFoodClass() == foodclass)
		{
			if (!strcmp(mFoodList[i]->GetId(), id))
			{
				delete mFoodList[i];
				for (int j = i; j < mProduct_Count - 1; j++)
				{
					mFoodList[j] = mFoodList[j + 1];
				}

				mFoodList[mProduct_Count - 1] = nullptr;
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

