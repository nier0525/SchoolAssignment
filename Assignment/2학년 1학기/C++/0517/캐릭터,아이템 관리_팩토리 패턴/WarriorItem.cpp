#include "WarriorItem.h"

WarriorItem* WarriorItem::pthis = nullptr;

WarriorItem* WarriorItem::GetInstance() {
	if (!pthis) {
		pthis = new WarriorItem();
	}
	return pthis;
}

void WarriorItem::Destory() {
	if (pthis) {
		delete pthis;
	}
}

WarriorItem::WarriorItem() {
	WarriorItemLIst = new ListNode<Item*>();
	SelectItem = nullptr;

	WarriorItemLIst->Insert(new Wenpon("DragonSword", 2000, 50));
	WarriorItemLIst->Insert(new Wenpon("DragonAxe", 2500, 70));

	WarriorItemLIst->Insert(new Armor("DragonHead", 2000, 30));
	WarriorItemLIst->Insert(new Armor("DragonShiled", 1500, 20));

	WarriorItemLIst->Insert(new Accessories("HPRing", 3000, 300 , 0));
}

WarriorItem::~WarriorItem() {
	if (WarriorItemLIst != nullptr) {
		delete WarriorItemLIst;
		WarriorItemLIst = nullptr;
	}
}

Item* WarriorItem::CreateWenpon() {
	SelectItem = WarriorItemLIst->GetHead()->GetNext();
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

	SelectItem = WarriorItemLIst->GetHead()->GetNext();
	while (SelectItem) {
		if (!strcmp(SelectItem->GetData()->GetItemName(),name) 
			&& SelectItem->GetData()->GetKind() == WENPON) {
			return new Wenpon(SelectItem->GetData()->GetItemName(), SelectItem->GetData()->GetPrice(), SelectItem->GetData()->GetFirstAbility());
		}
		SelectItem = SelectItem->GetNext();
	}
	cout << "���� ������ �Դϴ�." << endl;
	return nullptr;
}

Item* WarriorItem::CreateArmor() {
	SelectItem = WarriorItemLIst->GetHead()->GetNext();
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

	SelectItem = WarriorItemLIst->GetHead()->GetNext();
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

Item* WarriorItem::CreateAccessories() {
	SelectItem = WarriorItemLIst->GetHead()->GetNext();
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

	SelectItem = WarriorItemLIst->GetHead()->GetNext();
	while (SelectItem) {
		if (!strcmp(SelectItem->GetData()->GetItemName(), name)
			&& SelectItem->GetData()->GetKind() == ACCESSORIES) {
			return new Accessories(SelectItem->GetData()->GetItemName(), SelectItem->GetData()->GetPrice(), SelectItem->GetData()->GetFirstAbility(),SelectItem->GetData()->GetSecordAbility());
		}
		SelectItem = SelectItem->GetNext();
	}
	cout << "���� ������ �Դϴ�." << endl;
	return nullptr;
}