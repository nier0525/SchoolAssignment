#include "ItemStore.h"

const char* ItemStore::ItemName[] = { "" , "무기" , "방어구" , "장신구" };

ItemStore* ItemStore::pthis = nullptr;

ItemStore* ItemStore::GetInstance() {
	if (!pthis) {
		pthis = new ItemStore();
	}
	return pthis;
}

void ItemStore::Destory() {
	if (pthis) {
		delete pthis;
		pthis = nullptr;
	}
}

ItemStore::ItemStore() {
	ItemList = nullptr;
	SearchItem = nullptr;
}

ItemStore::~ItemStore() {
	if (ItemList != nullptr) {
		delete ItemList;
	}
}

void ItemStore::Init() {
	ItemList = new ListNode<Item*>();

	ItemList->Insert(new Wenpon("칠성검", 500, 25));
	ItemList->Insert(new Wenpon("집행검", 2000, 100));
	ItemList->Insert(new Wenpon("방천극", 1500, 75));

	ItemList->Insert(new Wenpon("1", 1500, 75));
	ItemList->Insert(new Armor("2", 2000, 48));
	ItemList->Insert(new Accessories("3", 1500, 0, 50));

	ItemList->Insert(new Armor("파르니르 장갑", 500, 12));
	ItemList->Insert(new Armor("파르니르 투구", 1500, 36));
	ItemList->Insert(new Armor("파르니르 갑옷", 2000, 48));

	ItemList->Insert(new Accessories("체력의 목걸이", 1500, 100, 0));
	ItemList->Insert(new Accessories("마력의 귀걸이", 1500, 0, 50));
}

void ItemStore::Sell() {
	int sel = Menu();

	SearchItem = ItemList->GetHead()->GetLink();
	while (SearchItem) {
		if (SearchItem->GetData()->GetKind() == sel) {
			cout << endl;
			cout << "이름 : " << SearchItem->GetData()->GetItemName() << endl;
			cout << "가격 : " << SearchItem->GetData()->GetPrice() << endl;
			switch (sel) {
			case WENPON:
				cout << "장착 효과 : 공격력 " << SearchItem->GetData()->GetFirstAbility() << " 증가" << endl;
				break;

			case ARMOR:
				cout << "장착 효과 : 방어력 " << SearchItem->GetData()->GetFirstAbility() << " 증가" << endl;
				break;

			case ACCESSORIES:
				cout << "장착 효과 : HP " << SearchItem->GetData()->GetFirstAbility() << " 증가" << endl;
				cout << "장착 효과 : MP " << SearchItem->GetData()->GetSecordAbility() << " 증가" << endl;
				break;
			}
			cout << endl;
		}
		SearchItem = SearchItem->GetLink();
	}

	char name[CHARSIZE];
	cout << "구입할 아이템 이름 : ";
	cin >> name;

	SearchItem = ItemList->GetHead()->GetLink();
	while (SearchItem) {
		if (!strcmp(SearchItem->GetData()->GetItemName(), name)
			&& SearchItem->GetData()->GetKind() == sel) {
			break;
		}	
		SearchItem = SearchItem->GetLink();
	}

	if (SearchItem == nullptr) {
		cout << "이름을 잘못 입력했습니다." << endl;
		return;
	}

	Item* item = nullptr;

	int money = SearchItem->GetData()->GetPrice();
	int first = SearchItem->GetData()->GetFirstAbility();
	int secord = SearchItem->GetData()->GetSecordAbility();

	switch (sel) {
	case WENPON:
		item = new Wenpon(name, money, first);
		break;

	case ARMOR:
		item = new Armor(name, money, first);
		break;

	case ACCESSORIES:
		item = new Accessories(name, money, first ,secord);
		break;
	}

	User* NowUser = LoginManager::GetInstance()->GetNowUser();

	if (!NowUser->Buy(item)) {
		cout << "돈이 부족합니다." << endl;
	}
	
	else {
		item->MakeCode();
		cout << item->GetItemName() << " 을 구입했습니다." << endl;
	}
	
	cout << NowUser->GetGameMoney() << " 원 남았습니다." << endl;
}

void ItemStore::Buy() {
	int sel = Menu();
	Item* item = nullptr;
	User* NowUser = LoginManager::GetInstance()->GetNowUser();
	bool flug = false;
	while (NowUser->GetItem(&item)) {
		if (item->GetKind() == sel) {
			cout << endl;
			cout << "이름 : " << item->GetItemName() << endl;
			cout << "아이템 코드 : " << item->GetItemCode() << endl;
			cout << "가격 : " << item->GetPrice() << endl;
			cout << endl;
			flug = true;
		}
	}

	NowUser->SearchItemEnd();

	if (!flug) {
		cout << "보유하고 있는 아이템이 없습니다." << endl;
		return;
	}

	char code[CHARSIZE];
	cout << "아이템 코드 : ";
	cin >> code;
	flug = false;

	while (NowUser->GetItem(&item)) {
		if (!strcmp(item->GetItemCode(), code) && item->GetKind() == sel) {
			cout << item->GetItemName() << " 을 팝니다." << endl;
			NowUser->Sell(item->GetItemCode());
			cout << NowUser->GetGameMoney() << " 원 남았습니다." << endl;
			flug = true;
			break;
		}
	}

	NowUser->SearchItemEnd();

	if (!flug) {
		cout << "아이템 코드를 잘못 입력했습니다." << endl;
	}
}

int ItemStore::Menu() {
	while (1) {
		int sel;
		cout << "< 아이템 선택 >" << endl;
		for (int i = 1; i < sizeof(ItemName) / sizeof(char*); i++) {
			cout << i << ". " << ItemName[i] << endl;
		}
		cout << "선택 : ";
		cin >> sel;

		if (cin.failbit) {
			cin.clear();
			cin.ignore(256, '\n');
		}

		if (sel < 1 || sel > sizeof(ItemName) / sizeof(char*) - 1) {
			cout << "< Error >" << endl;
			continue;
		}

		return sel;
	}
}