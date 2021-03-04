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

	printf("선택:");
	APPLIANCE_CLASS applianceclass;
	scanf("%d", &applianceclass);

	printf("이름:");
	char name[255];
	scanf("%s", name);

	if (!CheckData(strlen(name), PRODUCT_NAME_SIZE))
	{
		cout << "이름 입력 오류!! " << endl;
		return;
	}

	printf("모델명:");
	char model[255];
	scanf("%s", model);	

	if (!CheckData(strlen(model), APPLIANCE_MODEL_NUMBER_SIZE))
	{
		cout << "모델명 입력 오류!! " << endl;
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


	CAppliance* ptr = new CAppliance(applianceclass, name, model, price);
	ptr->MakeRecving_Date();
	ptr->MakeId();
	
	CDataBaseManager::InputData(ptr, APPLIANCE);
}

void CApplianceManager::SearchProduct()
{
	printf("검색할 가전제품 종류를 선택하세요\n");

	int count = CProduct_ID_Info::GetProductArrayCount(PRODUCT_APPLIANCE_COUNT);
	
	for(int i=0; i<count; i++)
	{
		printf("%d. %s :\n", i,CProduct_ID_Info::Product_Appliance_Class[i]);
	}

	printf("선택:");
	APPLIANCE_CLASS applianceclass;
	scanf("%d", &applianceclass);

	printf("검색상품이름:");
	char name[255];
	scanf("%s", name);

	printf("검색 제품 리스트\n");

	CProduct* ptr = CDataBaseManager::SearchData(APPLIANCE);

	if (!ptr)
	{
		printf("검색상품이 존재하지 않습니다.\n");
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
				printf("제품 아이디:%s 이름:%s  모델명:%s  가격:%d\n", fptr->GetId(), fptr->GetProductName(), fptr->GetModelNum(),
					 fptr->GetPrice());
			}
		}
		else
		{
			break; //예외처리 필요
		}

		ptr = CDataBaseManager::SearchData(APPLIANCE);
	}

	CDataBaseManager::SearchEnd();
}

void CApplianceManager::DeleteProduct()
{
	printf("삭제할 가전제품 종류를 선택하세요\n");

	int count = CProduct_ID_Info::GetProductArrayCount(PRODUCT_APPLIANCE_COUNT);
	
	for(int i=0; i<count; i++)
	{
		printf("%d. %s :\n", i,CProduct_ID_Info::Product_Appliance_Class[i]);
	}

	printf("선택:");
	APPLIANCE_CLASS applianceclass;
	scanf("%d", &applianceclass);

	printf("삭제상품이름:");
	char name[255];
	scanf("%s", name);	

	CProduct* ptr = CDataBaseManager::SearchData(APPLIANCE);

	if (!ptr)
	{
		printf("검색상품이 존재하지 않습니다.\n");
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
				printf("제품 아이디:%s 이름:%s  모델명:%s  가격:%d\n", fptr->GetId(), fptr->GetProductName(), fptr->GetModelNum(),
					fptr->GetPrice());
			}
		}
		else
		{
			break; //예외처리 필요
		}

		ptr = CDataBaseManager::SearchData(APPLIANCE);
	}

	CDataBaseManager::SearchEnd();

	printf("삭제상품아이디:");
	char id[255];
	scanf("%s", id);

	ptr = CDataBaseManager::SearchData(APPLIANCE);

	if (!ptr)
	{
		printf("검색상품이 존재하지 않습니다.\n");
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
					printf("선택상품이 삭제되었습니다 \n");
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

