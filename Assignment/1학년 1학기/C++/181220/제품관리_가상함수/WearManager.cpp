#include "WearManager.h"

CWearManager::CWearManager()
{
	//memset(mWearList, 0, sizeof(mWearList));
	mWearList = nullptr;
}

CWearManager::~CWearManager()
{
	if (mWearList != nullptr) {
		delete mWearList;
	}
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

	mWearList = new ListNode<CWear>;
	mWearList->Add(ptr);
	mProduct_Count++;
	//mWearList[mProduct_Count++] = ptr;
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

	for (int i = 0; i < mProduct_Count; i++)
	{
		if (mWearList->Data()->GetWearClass() == wearclass)
		{
			if (!strcmp(mWearList->Data()->GetProductName(), name))
			{
				printf("�̸�:%s  ������:%d  ����:%d\n", mWearList->Data()->GetProductName(), mWearList->Data()->GetSize(), mWearList->Data()->GetPrice());
			}
		}
	}
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

	bool flag = false;

	for (int i = 0; i < mProduct_Count; i++)
	{
		if (mWearList->Data()->GetWearClass() == wearclass)
		{
			if (!strcmp(mWearList->Data()->GetProductName(), name))
			{
				printf("�̸�:%s  id:%s  ������:%d ����:%d\n", mWearList->Data()->GetProductName(), mWearList->Data()->GetId(), mWearList->Data()->GetSize(), mWearList->Data()->GetPrice());
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
		if (mWearList->Data()->GetWearClass() == wearclass)
		{
			if (!strcmp(mWearList->Data()->GetId(), id))
			{
				delete mWearList;

				mWearList = nullptr;
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

