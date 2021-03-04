#include "FoodManager.h"

const char* CFoodManager::FoodName[] = { "��ä","����","����","�" };

CFoodManager::CFoodManager() {
	memset(ProductList, 0, sizeof(ProductList));
	NowProduct = nullptr;
}

CFoodManager::~CFoodManager() {
	//for (int i = 0; i < sizeof(FoodName) / sizeof(char*) + 1; i++) {
	//	if (ProductList[i] != nullptr) {
	//		delete ProductList[i];
	//		ProductList[i] = nullptr;
	//	}
	//}
}

int CFoodManager::Menu() {
	while (1) {
		cout << "<< ��ǰ ���� >>" << endl;
		for (int i = 0; i < sizeof(FoodName) / sizeof(char*); i++) {
			cout << i + 1 << ". " << FoodName[i] << endl;
		}
		int sel;
		cout << "���� : ";
		cin >> sel;

		if (cin.failbit) {
			cin.clear();
			cin.ignore(256, '\n');
		}

		if (sel <= NODATA || sel > sizeof(FoodName) / sizeof(char*)) {
			cout << "�߸� �Է��ϼ̽��ϴ�." << endl;
			continue;
		}

		return sel;
	}
}

void CFoodManager::init() {
	for (int i = 1; i < sizeof(FoodName) / sizeof(char*) + 1; i++) {
		ProductList[i] = new ListNode<CFood*>();
	}
}

void CFoodManager::Input() {
	int numb = Menu();
	
	char name[CHARSIZE];
	char gram[CHARSIZE];
	char making[CHARSIZE];
	char mid[CHARSIZE];

	int price;
	
	cout << "�̸� : "; cin >> name;
	cout << "���� : "; cin >> gram;
	cout << "�������� : "; cin >> making;
	cout << "���� : "; cin >> price;

	sprintf(mid, "%s%00d", ID_WEAR, ProductList[numb]->GetNodeCount() + 1);

	if (cin.failbit) {
		cin.clear();
		cin.ignore(256, '\n');
	}

	CFood* ptr = new CFood(name, making, mid, price);
	ptr->Setgram(gram);

	ProductList[numb]->Input(ptr);
}

void CFoodManager::Search() {
	int numb = Menu();

	char name[CHARSIZE];
	bool flug = false;
	cout << "�̸� : "; cin >> name;

	NowProduct = ProductList[numb]->Gethead();
	while (NowProduct != nullptr) {
		if (!strcmp(NowProduct->Getdata()->Getname(), name)) {
			cout << "�̸� : " << NowProduct->Getdata()->Getname() << " ";
			cout << "���� : " << NowProduct->Getdata()->Getgram() << " ";
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

void CFoodManager::Delete() {
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
			cout << "���� : " << NowProduct->Getdata()->Getgram() << " ";
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