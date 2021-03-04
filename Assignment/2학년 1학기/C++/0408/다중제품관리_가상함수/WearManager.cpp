#include "WearManager.h"

const char* CWearManager::WearName[] = { "�����Ƿ�","�����Ƿ�" };

CWearManager::CWearManager() {
	memset(ProductList, 0, sizeof(ProductList));
	NowProduct = nullptr;
}

CWearManager::~CWearManager() {
	//for (int i = 0; i < sizeof(WearName) / sizeof(char*) + 1; i++) {
	//	if (ProductList[i] != nullptr) {
	//		delete ProductList[i];
	//		ProductList[i] = nullptr;
	//	}
	//}
}

int CWearManager::Menu() {
	while (1) {
		cout << "<< ��ǰ ���� >>" << endl;
		for (int i = 0; i < sizeof(WearName) / sizeof(char*); i++) {
			cout << i + 1 << ". " << WearName[i] << endl;
		}
		int sel;
		cout << "���� : ";
		cin >> sel;

		if (cin.failbit) {
			cin.clear();
			cin.ignore(256, '\n');
		}

		if (sel <= NODATA || sel > sizeof(WearName) / sizeof(char*)) {
			cout << "�߸� �Է��ϼ̽��ϴ�." << endl;
			continue;
		}

		return sel;
	}
}

void CWearManager::init() {
	for (int i = 1; i < sizeof(WearName) / sizeof(char*) + 1; i++) {
		ProductList[i] = new ListNode<CWear*>();
	}
}

void CWearManager::Input() {
	int numb = Menu();

	char name[CHARSIZE];
	char size[CHARSIZE];
	char making[CHARSIZE];
	char mid[CHARSIZE];

	int price;

	cout << "�̸� : "; cin >> name;
	cout << "������ : "; cin >> size;
	cout << "�������� : "; cin >> making;
	cout << "���� : "; cin >> price;

	sprintf(mid, "%s%00d", ID_WEAR, ProductList[numb]->GetNodeCount() + 1);

	if (cin.failbit) {
		cin.clear();
		cin.ignore(256, '\n');
	}

	CWear* ptr = new CWear(name, making, mid, price);
	ptr->Setsize(size);

	ProductList[numb]->Input(ptr);
}

void CWearManager::Search() {
	int numb = Menu();

	char name[CHARSIZE];
	bool flug = false;

	cout << "�̸� : "; cin >> name;

	NowProduct = ProductList[numb]->Gethead();
	while (NowProduct != nullptr) {
		if (!strcmp(NowProduct->Getdata()->Getname(), name)) {
			cout << "�̸� : " << NowProduct->Getdata()->Getname() << " ";
			cout << "������ : " << NowProduct->Getdata()->Getsize() << " ";
			cout << "�������� : " << NowProduct->Getdata()->Getmakeday() << " ";
			cout << "���� : " << NowProduct->Getdata()->Getprice() << endl;
			flug = true;
		}
		NowProduct = NowProduct->Getlink();
	}
	if (!flug) {
		cout << "�ش� ��ǰ�� �����ϴ�." << endl;
	}
}

void CWearManager::Delete() {
	int numb = Menu();

	char name[CHARSIZE];
	char mid[CHARSIZE];
	bool flug = false;

	cout << "�̸� : "; cin >> name;

	NowProduct = ProductList[numb]->Gethead();
	while (NowProduct != nullptr) {
		if (!strcmp(NowProduct->Getdata()->Getname(), name)) {
			cout << "ID : " << NowProduct->Getdata()->Getmid() << " ";
			cout << "�̸� : " << NowProduct->Getdata()->Getname() << " ";
			cout << "������ : " << NowProduct->Getdata()->Getsize() << " ";
			cout << "�������� : " << NowProduct->Getdata()->Getmakeday() << " ";
			cout << "���� : " << NowProduct->Getdata()->Getprice() << endl;
			flug = true;
		}
		NowProduct = NowProduct->Getlink();
	}

	if (flug) {
		cout << "ID : "; cin >> mid;

		NowProduct = ProductList[numb]->Gethead();
		while (NowProduct != nullptr) {
			if (!strcmp(NowProduct->Getdata()->Getmid(), mid)) {
				cout << "�ش� ��ǰ�� �����Ǿ����ϴ�." << endl;
				ProductList[numb]->Delete(NowProduct);
				NowProduct = nullptr;
				return;
			}
			NowProduct = NowProduct->Getlink();
		}
	}

	cout << "�ش� ��ǰ�� �����ϴ�." << endl;
}