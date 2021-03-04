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

	CDataBaseManager::InputData(ptr, FOOD);
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

	CProduct* ptr = CDataBaseManager::SearchData(FOOD);

	if (!ptr)
	{
		printf("�˻���ǰ�� �������� �ʽ��ϴ�.\n");
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
					printf("��ǰ ���̵�:%s �̸�:%s  ����:%d  ����:%d\n", fptr->GetId(), fptr->GetProductName(),
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

	CProduct* ptr = CDataBaseManager::SearchData(FOOD);

	if (!ptr)
	{
		printf("�˻���ǰ�� �������� �ʽ��ϴ�.\n");
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
					printf("��ǰ ���̵�:%s �̸�:%s  ����:%d  ����:%d\n", fptr->GetId(), fptr->GetProductName(), fptr->GetWeight(),
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
		

	printf("������ǰ���̵�:");
	char id[255];
	scanf("%s", id);

	ptr = CDataBaseManager::SearchData(FOOD);

	if (!ptr)
	{
		printf("�˻���ǰ�� �������� �ʽ��ϴ�.\n");
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
					printf("���û�ǰ�� �����Ǿ����ϴ� \n");
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

