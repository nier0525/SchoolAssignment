#include "ApplianceManager.h"

CApplianceManager::CApplianceManager()
{
	
}

CApplianceManager::~CApplianceManager()
{
	
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
	
	CDataBaseManager::InputData(ptr, APPLIANCE);
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

	CProduct* ptr = CDataBaseManager::SearchData(APPLIANCE);

	if (!ptr)
	{
		printf("�˻���ǰ�� �������� �ʽ��ϴ�.\n");
		CDataBaseManager::SearchEnd();
		return;
	}

	while (ptr)
	{
		CAppliance* fptr = dynamic_cast<CAppliance*>(ptr);

		if (fptr)
		{

			if (!strcmp(fptr->GetProductName(), name))
			{
				printf("��ǰ ���̵�:%s �̸�:%s  �𵨸�:%s  ����:%d\n", fptr->GetId(), fptr->GetProductName(), fptr->GetModelNum(),
					 fptr->GetPrice());
			}
		}
		else
		{
			break; //����ó�� �ʿ�
		}

		ptr = CDataBaseManager::SearchData(APPLIANCE);
	}

	CDataBaseManager::SearchEnd();
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

	CProduct* ptr = CDataBaseManager::SearchData(APPLIANCE);

	if (!ptr)
	{
		printf("�˻���ǰ�� �������� �ʽ��ϴ�.\n");
		CDataBaseManager::SearchEnd();
		return;
	}

	while (ptr)
	{
		CAppliance* fptr = dynamic_cast<CAppliance*>(ptr);

		if (fptr)
		{

			if (!strcmp(fptr->GetProductName(), name))
			{
				printf("��ǰ ���̵�:%s �̸�:%s  �𵨸�:%s  ����:%d\n", fptr->GetId(), fptr->GetProductName(), fptr->GetModelNum(),
					fptr->GetPrice());
			}
		}
		else
		{
			break; //����ó�� �ʿ�
		}

		ptr = CDataBaseManager::SearchData(APPLIANCE);
	}

	CDataBaseManager::SearchEnd();

	printf("������ǰ���̵�:");
	char id[255];
	scanf("%s", id);

	ptr = CDataBaseManager::SearchData(APPLIANCE);

	if (!ptr)
	{
		printf("�˻���ǰ�� �������� �ʽ��ϴ�.\n");
		return;
	}

	while (ptr)
	{
		CAppliance* fptr = dynamic_cast<CAppliance*>(ptr);
		if (fptr)
		{

			if (!strcmp(fptr->GetId(), id))
			{
				if (CDataBaseManager::DeleteData(ptr, APPLIANCE))
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


		ptr = CDataBaseManager::SearchData(APPLIANCE);
	}

	CDataBaseManager::SearchEnd();
}

