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

	printf("����:");
	FOOD_CLASS foodclass;
	scanf("%d", &foodclass);

	printf("�̸�:");
	char name[PRODUCT_NAME_SIZE];
	scanf("%s", name);

	if (!CheckData(strlen(name), PRODUCT_NAME_SIZE))
	{
		cout << "�̸� �Է� ����!! " << endl;
		return;
	}

	printf("����:");
	int weight;
	scanf("%d", &weight);

	if (!CheckData(weight, FOOD_WEIGHT_MAX))
	{
		cout << "���� �Է� ����!! " << endl;
		return;
	}

	printf("����:");
	int price;
	scanf("%d", &price);

	if (!CheckData(price, PRICE_LEMIT))
	{
		cout << "���� �Է� ����!! " << endl;
		return;
	}


	CFood* ptr = new CFood(foodclass, name, price, weight);
	ptr->MakeRecving_Date();
	ptr->MakeId();

	mFoodList[mProduct_Count++] = ptr;
}

void CFoodManager::SearchProduct()
{
	printf("�˻��� ��ǰ ������ �����ϼ���\n");

	int count = CProduct_ID_Info::GetProductArrayCount(PRODUCT_FOOD_COUNT);

	for (int i = 0; i < count; i++)
	{
		printf("%d. %s :\n", i, CProduct_ID_Info::Product_Food_Class[i]);
	}

	printf("����:");
	FOOD_CLASS foodclass;
	scanf("%d", &foodclass);

	printf("�˻���ǰ�̸�:");
	char name[255];
	scanf("%s", name);

	printf("�˻� ��ǰ\n");

	for (int i = 0; i < mProduct_Count; i++)
	{
		if (mFoodList[i]->GetFoodClass() == foodclass)
		{
			if (!strcmp(mFoodList[i]->GetProductName(), name))
			{
				printf("�̸�:%s  ����:%d ����:%d\n", mFoodList[i]->GetProductName(),  mFoodList[i]->GetPrice(), mFoodList[i]->GetWeight());
			}
		}
	}
}

void CFoodManager::DeleteProduct()
{
	printf("������ ��ǰ ������ �����ϼ���\n");

	int count = CProduct_ID_Info::GetProductArrayCount(PRODUCT_FOOD_COUNT);

	for (int i = 0; i < count; i++)
	{
		printf("%d. %s :\n", i, CProduct_ID_Info::Product_Food_Class[i]);
	}

	printf("����:");
	FOOD_CLASS foodclass;
	scanf("%d", &foodclass);

	printf("������ǰ�̸�:");
	char name[PRODUCT_NAME_SIZE];
	scanf("%s", name);

	bool flag = false;

	for (int i = 0; i < mProduct_Count; i++)
	{
		if (mFoodList[i]->GetFoodClass() == foodclass)
		{
			if (!strcmp(mFoodList[i]->GetProductName(), name))
			{
				printf("�̸�:%s  id:%s ����:%d\n", mFoodList[i]->GetProductName(),  mFoodList[i]->GetId(), mFoodList[i]->GetPrice());
				flag = true;
			}
		}
	}

	if (!flag)
	{
		printf("�ش� ��ǰ�� �����ϴ�.\n");
		return;
	}
		

	printf("������ǰ���̵�:");
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
		printf("�ش� ��ǰ�� �����ϴ�.\n");
	}
}

