#include "ArcherItem.h"

ArcherItem* ArcherItem::pthis = nullptr;

ArcherItem* ArcherItem::GetInstance() {
	if (!pthis) {
		pthis = new ArcherItem();
	}
	return pthis;
}

void ArcherItem::Destory() {
	if (pthis) {
		delete pthis;
	}
}

ArcherItem::ArcherItem() {
	ArcherItemLIst = new ListNode<Item*>();
	SelectItem = nullptr;

	ArcherItemLIst->Insert(new Wenpon("DesertBow", 2000, 50));
	ArcherItemLIst->Insert(new Wenpon("MoonRightBow", 2500, 70));

	ArcherItemLIst->Insert(new Armor("WendyHead", 2000, 30));
	ArcherItemLIst->Insert(new Armor("WendyShiled", 1500, 20));

	ArcherItemLIst->Insert(new Accessories("BalanceRing", 3000, 150, 75));
}

ArcherItem::~ArcherItem() {
	if (ArcherItemLIst != nullptr) {
		delete ArcherItemLIst;
		ArcherItemLIst = nullptr;
	}
}

Item* ArcherItem::CreateWenpon() {
	SelectItem = ArcherItemLIst->GetHead()->GetNext();
	cout << "< ���� >" << endl;
	while (SelectItem) {
		if (SelectItem->GetData()->GetKind() == WENPON) {
			cout << endl;
			cout << "�̸� : " << SelectItem->GetData()->GetItemName() << endl;
			cout << "���� : " << SelectItem->GetData()->GetPrice() << endl;
			cout << "���� ȿ�� : ���ݷ� " << SelectItem->GetData()->GetFirstAbility() << " ����" << endl;
			cout << endl;
		}
		SelectItem = SelectItem->GetNext();
	}

	char name[CHARSIZE];
	cout << "������ �̸� : ";
	cin >> name;

	SelectItem = ArcherItemLIst->GetHead()->GetNext();
	while (SelectItem) {
		if (!strcmp(SelectItem->GetData()->GetItemName(), name)
			&& SelectItem->GetData()->GetKind() == WENPON) {
			return new Wenpon(SelectItem->GetData()->GetItemName(), SelectItem->GetData()->GetPrice(), SelectItem->GetData()->GetFirstAbility());
		}
		SelectItem = SelectItem->GetNext();
	}
	cout << "���� ������ �Դϴ�." << endl;
	return nullptr;
}

Item* ArcherItem::CreateArmor() {
	SelectItem = ArcherItemLIst->GetHead()->GetNext();
	cout << "< �� >" << endl;
	while (SelectItem) {
		if (SelectItem->GetData()->GetKind() == ARMOR) {
			cout << endl;
			cout << "�̸� : " << SelectItem->GetData()->GetItemName() << endl;
			cout << "���� : " << SelectItem->GetData()->GetPrice() << endl;
			cout << "���� ȿ�� : ���� " << SelectItem->GetData()->GetFirstAbility() << " ����" << endl;
			cout << endl;
		}
		SelectItem = SelectItem->GetNext();
	}

	char name[CHARSIZE];
	cout << "������ �̸� : ";
	cin >> name;

	SelectItem = ArcherItemLIst->GetHead()->GetNext();
	while (SelectItem) {
		if (!strcmp(SelectItem->GetData()->GetItemName(), name)
			&& SelectItem->GetData()->GetKind() == ARMOR) {
			return new Armor(SelectItem->GetData()->GetItemName(), SelectItem->GetData()->GetPrice(), SelectItem->GetData()->GetFirstAbility());
		}
		SelectItem = SelectItem->GetNext();
	}
	cout << "���� ������ �Դϴ�." << endl;
	return nullptr;
}

Item* ArcherItem::CreateAccessories() {
	SelectItem = ArcherItemLIst->GetHead()->GetNext();
	cout << "< �Ǽ��縮 >" << endl;
	while (SelectItem) {
		if (SelectItem->GetData()->GetKind() == ACCESSORIES) {
			cout << endl;
			cout << "�̸� : " << SelectItem->GetData()->GetItemName() << endl;
			cout << "���� : " << SelectItem->GetData()->GetPrice() << endl;
			cout << "���� ȿ�� : ü�� " << SelectItem->GetData()->GetFirstAbility() << " ����" << endl;
			cout << "���� ȿ�� : ���� " << SelectItem->GetData()->GetSecordAbility() << " ����" << endl;
			cout << endl;
		}
		SelectItem = SelectItem->GetNext();
	}

	char name[CHARSIZE];
	cout << "������ �̸� : ";
	cin >> name;

	SelectItem = ArcherItemLIst->GetHead()->GetNext();
	while (SelectItem) {
		if (!strcmp(SelectItem->GetData()->GetItemName(), name)
			&& SelectItem->GetData()->GetKind() == ACCESSORIES) {
			return new Accessories(SelectItem->GetData()->GetItemName(), SelectItem->GetData()->GetPrice(), SelectItem->GetData()->GetFirstAbility(), SelectItem->GetData()->GetSecordAbility());
		}
		SelectItem = SelectItem->GetNext();
	}
	cout << "���� ������ �Դϴ�." << endl;
	return nullptr;
}