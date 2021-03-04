#include "FoodManager.h"

const char* cFoodManager::mFoodName[] = { "소고기","돼지고기","양고기" };

cFoodManager::cFoodManager() {
	memset(mFood, 0, sizeof(mFood));
	mFoodCount = 0;
}

cFoodManager::~cFoodManager() {
	for (int i = 0; i < mFoodCount; i++) {
		delete mFood[i];
	}
}

void cFoodManager::Show() {
	for (int i = 0; i < mFoodCount; i++) {
		cout << "제품명 : " << mFood[i]->GetFoodName() << endl;
		cout << "품질 : " << mFood[i]->GetRank() << "\t";
		cout << "무게 : " << mFood[i]->GetGram() << endl;
		cout << "단가 : " << mFood[i]->GetMoney() << "\t";
		cout << "수량 : " << mFood[i]->GetCount() << endl;
	}
}

void cFoodManager::Input() {
	int number;
	int price;
	int count;
	int rank;
	int gram;

	while (1) {
		cout << "<<제품리스트>>" << endl;
		for (int i = 0; i < (sizeof(mFoodName) / sizeof(char*)); i++) {
			cout << i + 1 << ". " << mFoodName[i] << endl;
		}
		cout << "제품 번호 : "; cin >> number;
		cout << "수량 : ";	cin >> count;

		bool flag = false;

		for (int i = 0; i < mFoodCount; i++) {
			if (!strcmp(mFood[i]->GetFoodName(), mFoodName[number - 1])) {
				int result = mFood[i]->InCount(count);
				cout << result << "개가 입고되었습니다." << endl;
				if (count != result) {
					cout << "입고 가능한 갯수가 다 찼습니다. 더 이상 입고가 불가능합니다." << endl;
				}
				flag = true;
				break;
			}
		}
		if (!flag) {
			cout << "단가 : ";
			cin >> price;
			cout << "품질(1 ~ 5) : ";
			cin >> rank;
			cout << "무게 : ";
			cin >> gram;

			cFood* Food = new cFood();
			if (Food->SetFoodName(mFoodName[number - 1]) &&
				Food->SetMoney(price) && Food->SetRand(rank) &&
				Food->SetGram(gram) && Food->SetCount(count)) {
				mFood[mFoodCount++] = Food;
			}
			else {
				cout << "제품 등록 조건이 맞지 않습니다." << endl;
				delete Food;
				continue;
			}
		}
		break;
	}
}

void cFoodManager::Output() {
	int count, number;
	while (1) {
		cout << "<<제품리스트>>" << endl;
		for (int i = 0; i < sizeof(mFoodName) / sizeof(char*); i++) {
			cout << i + 1 << ". " << mFoodName[i] << endl;
		}
		cout << "제품번호 : ";
		cin >> number;

		bool flag = false;

		for (int i = 0; i < mFoodCount; i++) {
			if (!strcmp(mFood[i]->GetFoodName(), mFoodName[number - 1])) {
				cout << "수량 : ";	cin >> count;
				int result = mFood[i]->DeCount(count);
				cout << result << "개가 출고되었습니다." << endl;
				if (count != result) {
					cout << "출고 가능한 제품이 없습니다." << endl;
				}
				if (mFood[i]->GetCount() == 0) {
					delete mFood[i];
					for (int j = i; j < mFoodCount - 1; j++) {
						mFood[j] = mFood[j + 1];
					}
					mFood[mFoodCount - 1] = nullptr;
					mFoodCount--;
				}
				flag = true;
				break;
			}
		}
		if (!flag) {
			cout << "없는 제품입니다. 다시 입력하세요" << endl;
			int yn;
			cout << "다른 출고 상품을 입력하시겠습니까? (Yes : 1, No : 2)";
			cin >> yn;
			if (yn == 1) {
				continue;
			}
		}
		break;
	}
}