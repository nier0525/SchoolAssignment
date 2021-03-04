#include "ItemStore.h"
#include "CharacterManager.h"
#include "WarriorItem.h"
#include "ArcherItem.h"
#include "WizardItem.h"

ItemStore* ItemStore::pthis = nullptr;

ItemStore* ItemStore::GetInstance() {
	if (!pthis) {
		pthis = new ItemStore();
	}
	WarriorItem::GetInstance();
	ArcherItem::GetInstance();
	WizardItem::GetInstance();
	return pthis;
}

void ItemStore::Destory() {
	if (pthis) {
		delete pthis;
		pthis = nullptr;
	}
	WarriorItem::Destory();
	ArcherItem::Destory();
	WizardItem::Destory();
}

ItemStore::ItemStore() {

}

ItemStore::~ItemStore() {

}

void ItemStore::Buy() {
	ItemFactory* Item_F = nullptr;
	switch (CharacterManager::GetInstance()->GetNowCharacter()->GetJobCode()) {
	case WARRIOR:
		Item_F = WarriorItem::GetInstance();
		break;

	case ARCHER:
		Item_F = ArcherItem::GetInstance();
		break;

	case WIZARD:
		Item_F = WizardItem::GetInstance();
		break;
	}

	while (1) {
		int sel;
		cout << "< 아이템 상점 > " << endl << "1. 무기" << endl << "2. 방어구" << endl << "3. 악세사리" << endl;
		cout << "선택 : ";
		cin >> sel;

		if (cin.failbit) {
			cin.clear();
			cin.ignore(256, '\n');
		}

		if (sel < 1 || sel > 3) {
			cout << "< Error >" << endl;
			continue;
		}

		if (!CharacterManager::GetInstance()->GetNowCharacter()->Buy(Item_F->Sell(sel))) {
			cout << "돈이 부족하거나, 잘못된 정보를 입력하셨습니다." << endl;
		}
		break;
	}
}

void ItemStore::Sell() {
	while (1) {
		int sel;
		Item* item;
		Character* NowCharacter = CharacterManager::GetInstance()->GetNowCharacter();

		cout << "< 아이템 상점 > " << endl << "1. 무기" << endl << "2. 방어구" << endl << "3. 악세사리" << endl;
		cout << "선택 : ";
		cin >> sel;

		if (cin.failbit) {
			cin.clear();
			cin.ignore(256, '\n');
		}

		if (sel < 1 || sel > 3) {
			cout << "< Error >" << endl;
			continue;
		}

		bool flug = false;
		while (NowCharacter->GetItem(&item)) {
			if (item->GetKind() == sel) {
				cout << endl;
				cout << "이름 : " << item->GetItemName() << endl;
				cout << "아이템 코드 : " << item->GetItemCode() << endl;
				cout << "가격 : " << item->GetPrice() << endl;
				cout << endl;
				flug = true;
			}
		}
		NowCharacter->SearchEnd();

		if (!flug) {
			cout << "보유하고 있는 아이템이 없습니다." << endl;
			return;
		}

		char code[CHARSIZE];
		cout << "아이템 코드 : ";
		cin >> code;
		flug = false;

		while (NowCharacter->GetItem(&item)) {
			if (!strcmp(item->GetItemCode(), code)) {
				NowCharacter->Sell(code);
				cout << NowCharacter->GetMoney() << " 원 남았습니다." << endl;
				flug = true;
				break;
			}
		}
		NowCharacter->SearchEnd();
		if (!flug) {
			cout << "아이템 코드를 잘못 입력했습니다." << endl;
		}
		break;
	}
}