#include "WearManager.h"

const char* CWearManager::WearName[] = { "남성의류","여성의류" };

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
		cout << "<< 제품 선택 >>" << endl;
		for (int i = 0; i < sizeof(WearName) / sizeof(char*); i++) {
			cout << i + 1 << ". " << WearName[i] << endl;
		}
		int sel;
		cout << "선택 : ";
		cin >> sel;

		if (cin.failbit) {
			cin.clear();
			cin.ignore(256, '\n');
		}

		if (sel <= NODATA || sel > sizeof(WearName) / sizeof(char*)) {
			cout << "잘못 입력하셨습니다." << endl;
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

	cout << "이름 : "; cin >> name;
	cout << "사이즈 : "; cin >> size;
	cout << "제조일자 : "; cin >> making;
	cout << "가격 : "; cin >> price;

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

	cout << "이름 : "; cin >> name;

	NowProduct = ProductList[numb]->Gethead();
	while (NowProduct != nullptr) {
		if (!strcmp(NowProduct->Getdata()->Getname(), name)) {
			cout << "이름 : " << NowProduct->Getdata()->Getname() << " ";
			cout << "사이즈 : " << NowProduct->Getdata()->Getsize() << " ";
			cout << "제조일자 : " << NowProduct->Getdata()->Getmakeday() << " ";
			cout << "가격 : " << NowProduct->Getdata()->Getprice() << endl;
			flug = true;
		}
		NowProduct = NowProduct->Getlink();
	}
	if (!flug) {
		cout << "해당 제품이 없습니다." << endl;
	}
}

void CWearManager::Delete() {
	int numb = Menu();

	char name[CHARSIZE];
	char mid[CHARSIZE];
	bool flug = false;

	cout << "이름 : "; cin >> name;

	NowProduct = ProductList[numb]->Gethead();
	while (NowProduct != nullptr) {
		if (!strcmp(NowProduct->Getdata()->Getname(), name)) {
			cout << "ID : " << NowProduct->Getdata()->Getmid() << " ";
			cout << "이름 : " << NowProduct->Getdata()->Getname() << " ";
			cout << "사이즈 : " << NowProduct->Getdata()->Getsize() << " ";
			cout << "제조일자 : " << NowProduct->Getdata()->Getmakeday() << " ";
			cout << "가격 : " << NowProduct->Getdata()->Getprice() << endl;
			flug = true;
		}
		NowProduct = NowProduct->Getlink();
	}

	if (flug) {
		cout << "ID : "; cin >> mid;

		NowProduct = ProductList[numb]->Gethead();
		while (NowProduct != nullptr) {
			if (!strcmp(NowProduct->Getdata()->Getmid(), mid)) {
				cout << "해당 제품이 삭제되었습니다." << endl;
				ProductList[numb]->Delete(NowProduct);
				NowProduct = nullptr;
				return;
			}
			NowProduct = NowProduct->Getlink();
		}
	}

	cout << "해당 제품이 없습니다." << endl;
}