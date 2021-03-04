#include "ItemManager.h"

const char* CItemManager::ItemName[] = { "TV","�����","��Ź��" };

CItemManager::CItemManager() {
	memset(ProductList, 0, sizeof(ProductList));
	NowProduct = nullptr;
}

CItemManager::~CItemManager() {
	//for (int i = 0; i < sizeof(ItemName) / sizeof(char*) + 1; i++) {
	//	if (ProductList[i] != nullptr) {
	//		delete ProductList[i];
	//		ProductList[i] = nullptr;
	//	}
	//}
}

int CItemManager::Menu() {
	while (1) {
		cout << "<< ��ǰ ���� >>" << endl;
		for (int i = 0; i < sizeof(ItemName) / sizeof(char*); i++) {
			cout << i + 1 << ". " << ItemName[i] << endl;
		}
		int sel;
		cout << "���� : ";
		cin >> sel;

		if (cin.failbit) {
			cin.clear();
			cin.ignore(256, '\n');
		}

		if (sel <= NODATA || sel > sizeof(ItemName) / sizeof(char*)) {
			cout << "�߸� �Է��ϼ̽��ϴ�." << endl;
			continue;
		}

		return sel;
	}
}

void CItemManager::init() {
	for (int i = 1; i < sizeof(ItemName) / sizeof(char*) + 1; i++) {
		ProductList[i] = new ListNode<CItem*>();
	}
}

void CItemManager::Input() {
	int numb = Menu();

	char name[CHARSIZE];
	char id[CHARSIZE];
	char making[CHARSIZE];
	char mid[CHARSIZE];
	
	int price;

	cout << "�̸� : "; cin >> name;
	cout << "�𵨸� : "; cin >> id;
	cout << "�������� : "; cin >> making;
	cout << "���� : "; cin >> price;

	sprintf(mid, "%s%00d", ID_WEAR, ProductList[numb]->GetNodeCount() + 1);

	if (cin.failbit) {
		cin.clear();
		cin.ignore(256, '\n');
	}

	CItem* ptr = new CItem(name, making, mid, price);
	ptr->Setid(id);

	ProductList[numb]->Input(ptr);
}

void CItemManager::Search() {
	int numb = Menu();

	char name[CHARSIZE];
	bool flug = false;

	cout << "�̸� : "; cin >> name;

	NowProduct = ProductList[numb]->Gethead();
	while (NowProduct != nullptr) {
		if (!strcmp(NowProduct->Getdata()->Getname(), name)) {
			cout << "�̸� : " << NowProduct->Getdata()->Getname() << " ";
			cout << "�𵨸� : " << NowProduct->Getdata()->Getid() << " ";
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

void CItemManager::Delete() {
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
			cout << "�𵨸� : " << NowProduct->Getdata()->Getid() << " ";
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