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

	printf("����:");
	APPLIANCE_CLASS applianceclass;
	scanf("%d", &applianceclass);

	printf("�̸�:");
	char name[255];
	scanf("%s", name);

	if (!CheckData(strlen(name), PRODUCT_NAME_SIZE))
	{
		cout << "�̸� �Է� ����!! " << endl;
		return;
	}

	printf("�𵨸�:");
	char model[255];
	scanf("%s", model);	

	if (!CheckData(strlen(model), APPLIANCE_MODEL_NUMBER_SIZE))
	{
		cout << "�𵨸� �Է� ����!! " << endl;
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


	CAppliance* ptr = new CAppliance(applianceclass, name, model, price);
	ptr->MakeRecving_Date();
	ptr->MakeId();
	
	mApplianceList[mProduct_Count++] = ptr;
}

void CApplianceManager::SearchProduct()
{
	printf("�˻��� ������ǰ ������ �����ϼ���\n");

	int count = CProduct_ID_Info::GetProductArrayCount(PRODUCT_APPLIANCE_COUNT);
	
	for(int i=0; i<count; i++)
	{
		printf("%d. %s :\n", i,CProduct_ID_Info::Product_Appliance_Class[i]);
	}

	printf("����:");
	APPLIANCE_CLASS applianceclass;
	scanf("%d", &applianceclass);

	printf("�˻���ǰ�̸�:");
	char name[255];
	scanf("%s", name);

	printf("�˻� ��ǰ ����Ʈ\n");

	for (int i = 0; i < mProduct_Count; i++)
	{
		if (mApplianceList[i]->GetApplianceClass() == applianceclass)
		{
			if (!strcmp(mApplianceList[i]->GetProductName(), name))
			{
				printf("�̸�:%s  �𵨸�:%s  ����:%d\n", mApplianceList[i]->GetProductName(), mApplianceList[i]->GetModelNum(), mApplianceList[i]->GetPrice());
			}
		}
	}	
}

void CApplianceManager::DeleteProduct()
{
	printf("������ ������ǰ ������ �����ϼ���\n");

	int count = CProduct_ID_Info::GetProductArrayCount(PRODUCT_APPLIANCE_COUNT);
	
	for(int i=0; i<count; i++)
	{
		printf("%d. %s :\n", i,CProduct_ID_Info::Product_Appliance_Class[i]);
	}

	printf("����:");
	APPLIANCE_CLASS applianceclass;
	scanf("%d", &applianceclass);

	printf("������ǰ�̸�:");
	char name[255];
	scanf("%s", name);	

	bool flag = false;

	for (int i = 0; i < mProduct_Count; i++)
	{
		if (mApplianceList[i]->GetApplianceClass() == applianceclass)
		{
			if (!strcmp(mApplianceList[i]->GetProductName(), name))
			{
				printf("�̸�:%s  �𵨸�:%s  id:%s ����:%d\n", mApplianceList[i]->GetProductName(), mApplianceList[i]->GetModelNum(), mApplianceList[i]->GetId(), mApplianceList[i]->GetPrice());
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
		printf("�ش� ��ǰ�� �����ϴ�.\n");		
	}
}

