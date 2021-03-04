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
		cout << "< ������ ���� > " << endl << "1. ����" << endl << "2. ��" << endl << "3. �Ǽ��縮" << endl;
		cout << "���� : ";
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
			cout << "���� �����ϰų�, �߸��� ������ �Է��ϼ̽��ϴ�." << endl;
		}
		break;
	}
}

void ItemStore::Sell() {
	while (1) {
		int sel;
		Item* item;
		Character* NowCharacter = CharacterManager::GetInstance()->GetNowCharacter();

		cout << "< ������ ���� > " << endl << "1. ����" << endl << "2. ��" << endl << "3. �Ǽ��縮" << endl;
		cout << "���� : ";
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
				cout << "�̸� : " << item->GetItemName() << endl;
				cout << "������ �ڵ� : " << item->GetItemCode() << endl;
				cout << "���� : " << item->GetPrice() << endl;
				cout << endl;
				flug = true;
			}
		}
		NowCharacter->SearchEnd();

		if (!flug) {
			cout << "�����ϰ� �ִ� �������� �����ϴ�." << endl;
			return;
		}

		char code[CHARSIZE];
		cout << "������ �ڵ� : ";
		cin >> code;
		flug = false;

		while (NowCharacter->GetItem(&item)) {
			if (!strcmp(item->GetItemCode(), code)) {
				NowCharacter->Sell(code);
				cout << NowCharacter->GetMoney() << " �� ���ҽ��ϴ�." << endl;
				flug = true;
				break;
			}
		}
		NowCharacter->SearchEnd();
		if (!flug) {
			cout << "������ �ڵ带 �߸� �Է��߽��ϴ�." << endl;
		}
		break;
	}
}