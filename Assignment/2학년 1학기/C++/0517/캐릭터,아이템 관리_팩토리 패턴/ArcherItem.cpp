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
	cout << "< 무기 >" << endl;
	while (SelectItem) {
		if (SelectItem->GetData()->GetKind() == WENPON) {
			cout << endl;
			cout << "이름 : " << SelectItem->GetData()->GetItemName() << endl;
			cout << "가격 : " << SelectItem->GetData()->GetPrice() << endl;
			cout << "장착 효과 : 공격력 " << SelectItem->GetData()->GetFirstAbility() << " 증가" << endl;
			cout << endl;
		}
		SelectItem = SelectItem->GetNext();
	}

	char name[CHARSIZE];
	cout << "아이템 이름 : ";
	cin >> name;

	SelectItem = ArcherItemLIst->GetHead()->GetNext();
	while (SelectItem) {
		if (!strcmp(SelectItem->GetData()->GetItemName(), name)
			&& SelectItem->GetData()->GetKind() == WENPON) {
			return new Wenpon(SelectItem->GetData()->GetItemName(), SelectItem->GetData()->GetPrice(), SelectItem->GetData()->GetFirstAbility());
		}
		SelectItem = SelectItem->GetNext();
	}
	cout << "없는 아이템 입니다." << endl;
	return nullptr;
}

Item* ArcherItem::CreateArmor() {
	SelectItem = ArcherItemLIst->GetHead()->GetNext();
	cout << "< 방어구 >" << endl;
	while (SelectItem) {
		if (SelectItem->GetData()->GetKind() == ARMOR) {
			cout << endl;
			cout << "이름 : " << SelectItem->GetData()->GetItemName() << endl;
			cout << "가격 : " << SelectItem->GetData()->GetPrice() << endl;
			cout << "장착 효과 : 방어력 " << SelectItem->GetData()->GetFirstAbility() << " 증가" << endl;
			cout << endl;
		}
		SelectItem = SelectItem->GetNext();
	}

	char name[CHARSIZE];
	cout << "아이템 이름 : ";
	cin >> name;

	SelectItem = ArcherItemLIst->GetHead()->GetNext();
	while (SelectItem) {
		if (!strcmp(SelectItem->GetData()->GetItemName(), name)
			&& SelectItem->GetData()->GetKind() == ARMOR) {
			return new Armor(SelectItem->GetData()->GetItemName(), SelectItem->GetData()->GetPrice(), SelectItem->GetData()->GetFirstAbility());
		}
		SelectItem = SelectItem->GetNext();
	}
	cout << "없는 아이템 입니다." << endl;
	return nullptr;
}

Item* ArcherItem::CreateAccessories() {
	SelectItem = ArcherItemLIst->GetHead()->GetNext();
	cout << "< 악세사리 >" << endl;
	while (SelectItem) {
		if (SelectItem->GetData()->GetKind() == ACCESSORIES) {
			cout << endl;
			cout << "이름 : " << SelectItem->GetData()->GetItemName() << endl;
			cout << "가격 : " << SelectItem->GetData()->GetPrice() << endl;
			cout << "장착 효과 : 체력 " << SelectItem->GetData()->GetFirstAbility() << " 증가" << endl;
			cout << "장착 효과 : 마나 " << SelectItem->GetData()->GetSecordAbility() << " 증가" << endl;
			cout << endl;
		}
		SelectItem = SelectItem->GetNext();
	}

	char name[CHARSIZE];
	cout << "아이템 이름 : ";
	cin >> name;

	SelectItem = ArcherItemLIst->GetHead()->GetNext();
	while (SelectItem) {
		if (!strcmp(SelectItem->GetData()->GetItemName(), name)
			&& SelectItem->GetData()->GetKind() == ACCESSORIES) {
			return new Accessories(SelectItem->GetData()->GetItemName(), SelectItem->GetData()->GetPrice(), SelectItem->GetData()->GetFirstAbility(), SelectItem->GetData()->GetSecordAbility());
		}
		SelectItem = SelectItem->GetNext();
	}
	cout << "없는 아이템 입니다." << endl;
	return nullptr;
}