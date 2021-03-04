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

	SelectItem = WarriorItemLIst->GetHead()->GetNext();
	while (SelectItem) {
		if (!strcmp(SelectItem->GetData()->GetItemName(),name) 
			&& SelectItem->GetData()->GetKind() == WENPON) {
			return new Wenpon(SelectItem->GetData()->GetItemName(), SelectItem->GetData()->GetPrice(), SelectItem->GetData()->GetFirstAbility());
		}
		SelectItem = SelectItem->GetNext();
	}
	cout << "없는 아이템 입니다." << endl;
	return nullptr;
}

Item* WarriorItem::CreateArmor() {
	SelectItem = WarriorItemLIst->GetHead()->GetNext();
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

	SelectItem = WarriorItemLIst->GetHead()->GetNext();
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

Item* WarriorItem::CreateAccessories() {
	SelectItem = WarriorItemLIst->GetHead()->GetNext();
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

	SelectItem = WarriorItemLIst->GetHead()->GetNext();
	while (SelectItem) {
		if (!strcmp(SelectItem->GetData()->GetItemName(), name)
			&& SelectItem->GetData()->GetKind() == ACCESSORIES) {
			return new Accessories(SelectItem->GetData()->GetItemName(), SelectItem->GetData()->GetPrice(), SelectItem->GetData()->GetFirstAbility(),SelectItem->GetData()->GetSecordAbility());
		}
		SelectItem = SelectItem->GetNext();
	}
	cout << "없는 아이템 입니다." << endl;
	return nullptr;
}