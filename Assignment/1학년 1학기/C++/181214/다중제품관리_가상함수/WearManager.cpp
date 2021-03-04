#include "WearManager.h"

const char* cWearManager::mWearName[] = { "반팔","긴팔","자켓" };

const char* cWearManager::mWearSex[] = { "남성", "여성" };

cWearManager::cWearManager() {
	memset(mWear, 0, sizeof(mWear));
	mWearCount = 0;
}

cWearManager::~cWearManager() {
	for (int i = 0; i < mWearCount; i++) {
		delete mWear[i];
	}
}

void cWearManager::Show() {
	for (int i = 0; i < mWearCount; i++) {
		cout << "제품명 : " << mWear[i]->GetWearName() << endl;
		cout << "성별 : " << mWear[i]->GetSex() << "\t";
		cout << "사이즈 : " << mWear[i]->GetSize() << endl;
		cout << "단가 : " << mWear[i]->GetMoney() << "\t";
		cout << "수량 : " << mWear[i]->GetCount() << endl;
	}
}

void cWearManager::Input() {
	int number;
	int price;
	int count;
	int rank;
	int gram;

	while (1) {
		cout << "<<제품리스트>>" << endl;
		for (int i = 0; i < (sizeof(mWearName) / sizeof(char*)); i++) {
			cout << i + 1 << ". " << mWearName[i] << endl;
		}
		cout << "제품 번호 : "; cin >> number;
		cout << "수량 : ";	cin >> count;

		bool flag = false;

		for (int i = 0; i < mWearCount; i++) {
			if (!strcmp(mWear[i]->GetWearName(), mWearName[number - 1])) {
				int result = mWear[i]->InCount(count);
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
			cout << "성별(1:남자,2:여자) : ";
			cin >> rank;
			cout << "무게(1:90 ~ 5:110) : ";
			cin >> gram;

			cWear* Wear = new cWear();
			if (Wear->SetWearName(mWearName[number - 1]) &&
				Wear->SetSex(mWearSex[rank - 1]) && 
				Wear->SetMoney(price) &&
				Wear->SetSize(gram) &&  
				Wear->SetCount(count)) 
			{
				mWear[mWearCount++] = Wear;
			}
			else {
				cout << "제품 등록 조건이 맞지 않습니다." << endl;
				delete Wear;
				continue;
			}
		}
		break;
	}
}

void cWearManager::Output() {
	int count, number;
	while (1) {
		cout << "<<제품리스트>>" << endl;
		for (int i = 0; i < sizeof(mWearName) / sizeof(char*); i++) {
			cout << i + 1 << ". " << mWearName[i] << endl;
		}
		cout << "제품번호 : ";
		cin >> number;

		bool flag = false;

		for (int i = 0; i < mWearCount; i++) {
			if (!strcmp(mWear[i]->GetWearName(), mWearName[number - 1])) {
				cout << "수량 : ";	cin >> count;
				int result = mWear[i]->DeCount(count);
				cout << result << "개가 출고되었습니다." << endl;
				if (count != result) {
					cout << "출고 가능한 제품이 없습니다." << endl;
				}
				if (mWear[i]->GetCount() == 0) {
					delete mWear[i];
					for (int j = i; j < mWearCount - 1; j++) {
						mWear[j] = mWear[j + 1];
					}
					mWear[mWearCount - 1] = nullptr;
					mWearCount--;
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