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

	printf("����:");
	WEAR_CLASS wearclass;
	scanf("%d", &wearclass);

	printf("�̸�:");
	char name[255];
	scanf("%s", name);

	if (!CheckData(strlen(name), PRODUCT_NAME_SIZE))
	{
		cout << "�̸� �Է� ����!! " << endl;
		return;
	}

	printf("������:");
	int size;
	scanf("%d", &size);

	printf("����:");
	int price;
	scanf("%d", &price);

	if (!CheckData(price, PRICE_LEMIT))
	{
		cout << "���� �Է� ����!! " << endl;
		return;
	}


	CWear* ptr = new CWear(wearclass, name, price, size);
	ptr->MakeRecving_Date();
	ptr->MakeId();

	CDataBaseManager::InputData(ptr, WEAR);
	
}

void CWearManager::SearchProduct()
{
	printf("�˻��� �Ƿ���ǰ ������ �����ϼ���\n");

	int count = CProduct_ID_Info::GetProductArrayCount(PRODUCT_WEAR_COUNT);

	for (int i = 0; i < count; i++)
	{
		printf("%d. %s :\n", i, CProduct_ID_Info::Product_Wear_Class[i]);
	}

	printf("����:");
	WEAR_CLASS wearclass;
	scanf("%d", &wearclass);

	printf("�˻���ǰ�̸�:");
	char name[255];
	scanf("%s", name);

	printf("�˻� ��ǰ ����Ʈ\n");

	CProduct* ptr = CDataBaseManager::SearchData(WEAR);
	if (!ptr)
	{
		printf("�˻���ǰ�� �������� �ʽ��ϴ�.\n");
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
					printf("�̸�:%s  ������:%d  ����:%d\n", fptr->GetProductName(), fptr->GetSize(), fptr->GetPrice());
				}
			}
		}
		ptr = CDataBaseManager::SearchData(WEAR);
	}

	CDataBaseManager::SearchEnd();
}

void CWearManager::DeleteProduct()
{
	printf("������ �Ƿ���ǰ ������ �����ϼ���\n");

	int count = CProduct_ID_Info::GetProductArrayCount(PRODUCT_WEAR_COUNT);

	for (int i = 0; i < count; i++)
	{
		printf("%d. %s :\n", i, CProduct_ID_Info::Product_Wear_Class[i]);
	}

	printf("����:");
	WEAR_CLASS wearclass;
	scanf("%d", &wearclass);

	printf("������ǰ�̸�:");
	char name[255];
	scanf("%s", name);

	CProduct* ptr = CDataBaseManager::SearchData(WEAR);

	if (!ptr)
	{
		printf("�˻���ǰ�� �������� �ʽ��ϴ�.\n");
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
					printf("��ǰ ���̵�:%s �̸�:%s  ������:%d  ����:%d\n", fptr->GetId(), fptr->GetProductName(), 
						fptr->GetSize(),  fptr->GetPrice());

				}
			}
		}
		ptr = CDataBaseManager::SearchData(WEAR);

	}

	CDataBaseManager::SearchEnd();

	printf("������ǰ���̵�:");
	char id[255];
	scanf("%s", id);

	ptr = CDataBaseManager::SearchData(WEAR);

	if (!ptr)
	{
		printf("�˻���ǰ�� �������� �ʽ��ϴ�.\n");
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
				printf("���û�ǰ�� �����Ǿ����ϴ� \n");
				break;
			}
		}

		ptr = CDataBaseManager::SearchData(FOOD);
	}

	CDataBaseManager::SearchEnd();
}

