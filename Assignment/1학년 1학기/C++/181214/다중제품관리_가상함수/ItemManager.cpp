#include "ItemManager.h"

const char* cItemManager::mItemName[] = { "프린트","컴퓨터","라디오" };

cItemManager::cItemManager() {
	memset(mItem, 0, sizeof(mItem));
	mItemCount = 0;
}

cItemManager::~cItemManager() {
	for (int i = 0; i < mItemCount; i++) {
		delete mItem[i];
	}
}

void cItemManager::Show() {
	for (int i = 0; i < mItemCount; i++) {
		cout << "제품명 : " << mItem[i]->GetItemName() << endl;
		cout << "일련번호 : 20181213" << mItem[i]->GetItemid() << "\t";
		cout << "무상수리기간(월) : " << mItem[i]->GetFreeAs() << endl;
		cout << "단가 : " << mItem[i]->GetMoney() << "\t";
		cout << "수량 : " << mItem[i]->GetCount() << endl;
	}
}

void cItemManager::Input() {
	int number;
	int price;
	int count;
	int rank;
	int gram;

	while (1) {
		cout << "<<제품리스트>>" << endl;
		for (int i = 0; i < (sizeof(mItemName) / sizeof(char*)); i++) {
			cout << i + 1 << ". " << mItemName[i] << endl;
		}
		cout << "제품 번호 : "; cin >> number;
		cout << "수량 : ";	cin >> count;

		bool flag = false;

		for (int i = 0; i < mItemCount; i++) {
			if (!strcmp(mItem[i]->GetItemName(), mItemName[number - 1])) {
				int result = mItem[i]->InCount(count);
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
			cout << "일련번호 : ";
			cin >> rank;
			cout << "무상수리기간(최대24개월) : ";
			cin >> gram;

			cItem* Item = new cItem();
			if (Item->SetItemName(mItemName[number - 1]) &&
				Item->SetMoney(price) && Item->SetItemid(rank) &&
				Item->SetFreeAs(gram) && Item->SetCount(count)) {
				mItem[mItemCount++] = Item;
			}
			else {
				cout << "제품 등록 조건이 맞지 않습니다." << endl;
				delete Item;
				continue;
			}
		}
		break;
	}
}

void cItemManager::Output() {
	int count, number;
	while (1) {
		cout << "<<제품리스트>>" << endl;
		for (int i = 0; i < sizeof(mItemName) / sizeof(char*); i++) {
			cout << i + 1 << ". " << mItemName[i] << endl;
		}
		cout << "제품번호 : ";
		cin >> number;

		bool flag = false;

		for (int i = 0; i < mItemCount; i++) {
			if (!strcmp(mItem[i]->GetItemName(), mItemName[number - 1])) {
				cout << "수량 : ";	cin >> count;
				int result = mItem[i]->DeCount(count);
				cout << result << "개가 출고되었습니다." << endl;
				if (count != result) {
					cout << "출고 가능한 제품이 없습니다." << endl;
				}
				if (mItem[i]->GetCount() == 0) {
					delete mItem[i];
					for (int j = i; j < mItemCount - 1; j++) {
						mItem[j] = mItem[j + 1];
					}
					mItem[mItemCount - 1] = nullptr;
					mItemCount--;
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