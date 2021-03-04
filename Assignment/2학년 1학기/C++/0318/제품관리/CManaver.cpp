#include "CManager.h"

const char* cManager::mNameList[] = { "사과","배","바나나","자몽","포도" };

cManager::cManager() {
	memset(ListProduct, 0, sizeof(ListProduct));
	mListCount = 0;
	product = nullptr;
}

cManager::~cManager() {
	for (int i = 0; i < mListCount; i++) {
		if (ListProduct[i] != nullptr) {
			delete ListProduct[i];
			ListProduct[i] = nullptr;
		}
	}
	delete product;	
}

void cManager::CinCheak() {
	if (cin.failbit) {
		cin.clear();
		cin.ignore(256, '\n'); // while (getchar() != '\n');
	}
}

void cManager::Run() {
	while (1) {
		int sel;

		cout << "< 제품관리 시스템 > " << endl << "1. 제품 입고" << endl << "2. 제품 출고" << endl << "3. 제품 출력" << endl << "4 .종료" << endl;
		cout << "선택 : ";
		cin >> sel;

		switch (sel) {
		case INPUT:
			Add();
			break;

		case OUTPUT:
			Del();
			break;

		case DISPLAY:
			Display();
			break;

		case EXIT:
			return;
			break;
		}
	}
}

void cManager::Add() {
	int numb;
	int count;
	int money;

	bool flug = false;

	while (1) {

		cout << "< 제품 리스트 >" << endl;

		for (int i = 0; i < (sizeof(mNameList) / sizeof(char*)); i++) {
			cout << i + 1 << ". " << mNameList[i] << endl;
		}

		cout << "제품 번호 : ";
		cin >> numb;
		CinCheak();

		cout << "제품 수량 : ";
		cin >> count;
		CinCheak();

		for (int i = 0; i < mListCount; i++) {
			if (!strcmp(ListProduct[i]->GetName(), mNameList[numb - 1])) {
				int index = ListProduct[i]->Add(count);
				cout << index << " 개가 입고되었습니다." << endl;
				
				if (index != count) {
					cout << "더 이상 입고가 불가능합니다." << endl;
				}
				flug = true;
				break;
			}
		}

		if (!flug) {
			cout << "제품 가격 : ";
			cin >> money;
			CinCheak();

			product = new cProduct();

			if (product->SetName(mNameList[numb - 1]) && product->SetCount(count) && product->SetMoney(money)) {
				ListProduct[mListCount++] = product;
				product = nullptr;
			}
			else {
				cout << "제품 등록 조건이 맞지 않습니다." << endl;
				delete product;
				continue;
			}
		}
		break;
	}
}

void cManager::Del() {
	int count;
	int numb;

	bool flug = false;

	while (1) {
		cout << "< 제품 리스트 >" << endl;

		for (int i = 0; i < (sizeof(mNameList) / sizeof(char*)); i++) {
			cout << i + 1 << ". " << mNameList[i] << endl;
		}

		cout << "제품 번호 : ";
		cin >> numb;
		CinCheak();

		for (int i = 0; i < mListCount; i++) {
			if (!strcmp(ListProduct[i]->GetName(), mNameList[numb - 1])) {

				cout << "제품 수량 : ";
				cin >> count;
				CinCheak();


				int index = ListProduct[i]->Delete(count);
				cout << index << " 개가 출고 되었습니다." << endl;
				
				if (count != index) {
					cout << "출고 가능한 제품이 없습니다." << endl;
				}

				if (ListProduct[i]->GetCount() == 0) {
					delete ListProduct[i];
					for (int j = i; j < mListCount - 1; j++) {
						ListProduct[j] = ListProduct[j + 1];
					}
					ListProduct[mListCount - 1] = nullptr;
					mListCount--;
				}
				flug = true;
				break;
			}
		}
		if (!flug) {
			cout << "없는 제품입니다. 다시 입력하시겠습니까?" << endl;
			int sel;
			cout << "예(1) / 아니오(2) : ";
			cin >> sel;

			if (sel == 1) {
				continue;
			}
		}
		break;
	}
}

void cManager::Display() {
	for (int i = 0; i < mListCount; i++) {
		cout << ListProduct[i]->GetName() << endl;
		cout << "개수 : " << ListProduct[i]->GetCount() << endl;
		cout << "가격 : " << ListProduct[i]->GetMoney() << endl;
	}
	if (mListCount == 0) {
		cout << "현재 제품의 재고가 없습니다." << endl;
	}
}