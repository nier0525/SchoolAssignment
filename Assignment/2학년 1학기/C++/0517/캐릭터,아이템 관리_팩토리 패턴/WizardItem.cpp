#include "WizardItem.h"

WizardItem* WizardItem::pthis = nullptr;

WizardItem* WizardItem::GetInstance() {
	if (!pthis) {
		pthis = new WizardItem();
	}
	return pthis;
}

void WizardItem::Destory() {
	if (pthis) {
		delete pthis;
	}
}

WizardItem::WizardItem() {
	WizardItemLIst = new ListNode<Item*>();
	SelectItem = nullptr;

	WizardItemLIst->Insert(new Wenpon("TimelessWand", 2000, 50));
	WizardItemLIst->Insert(new Wenpon("ReverseWand", 2500, 70));

	WizardItemLIst->Insert(new Armor("TimelessHead", 2000, 30));
	WizardItemLIst->Insert(new Armor("ReverseShiled", 1500, 20));

	WizardItemLIst->Insert(new Accessories("MPRing", 3000, 0, 150));
}

WizardItem::~WizardItem() {
	if (WizardItemLIst != nullptr) {
		delete WizardItemLIst;
		WizardItemLIst = nullptr;
	}
}

Item* WizardItem::CreateWenpon() {
	SelectItem = WizardItemLIst->GetHead()->GetNext();
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

	SelectItem = WizardItemLIst->GetHead()->GetNext();
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

Item* WizardItem::CreateArmor() {
	SelectItem = WizardItemLIst->GetHead()->GetNext();
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

	SelectItem = WizardItemLIst->GetHead()->GetNext();
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

Item* WizardItem::CreateAccessories() {
	SelectItem = WizardItemLIst->GetHead()->GetNext();
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

	SelectItem = WizardItemLIst->GetHead()->GetNext();
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