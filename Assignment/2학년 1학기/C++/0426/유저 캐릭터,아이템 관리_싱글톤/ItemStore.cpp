#include "ItemStore.h"

const char* ItemStore::ItemName[] = { "" , "����" , "��" , "��ű�" };

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

	ItemList->Insert(new Wenpon("ĥ����", 500, 25));
	ItemList->Insert(new Wenpon("�����", 2000, 100));
	ItemList->Insert(new Wenpon("��õ��", 1500, 75));

	ItemList->Insert(new Wenpon("1", 1500, 75));
	ItemList->Insert(new Armor("2", 2000, 48));
	ItemList->Insert(new Accessories("3", 1500, 0, 50));

	ItemList->Insert(new Armor("�ĸ��ϸ� �尩", 500, 12));
	ItemList->Insert(new Armor("�ĸ��ϸ� ����", 1500, 36));
	ItemList->Insert(new Armor("�ĸ��ϸ� ����", 2000, 48));

	ItemList->Insert(new Accessories("ü���� �����", 1500, 100, 0));
	ItemList->Insert(new Accessories("������ �Ͱ���", 1500, 0, 50));
}

void ItemStore::Sell() {
	int sel = Menu();

	SearchItem = ItemList->GetHead()->GetLink();
	while (SearchItem) {
		if (SearchItem->GetData()->GetKind() == sel) {
			cout << endl;
			cout << "�̸� : " << SearchItem->GetData()->GetItemName() << endl;
			cout << "���� : " << SearchItem->GetData()->GetPrice() << endl;
			switch (sel) {
			case WENPON:
				cout << "���� ȿ�� : ���ݷ� " << SearchItem->GetData()->GetFirstAbility() << " ����" << endl;
				break;

			case ARMOR:
				cout << "���� ȿ�� : ���� " << SearchItem->GetData()->GetFirstAbility() << " ����" << endl;
				break;

			case ACCESSORIES:
				cout << "���� ȿ�� : HP " << SearchItem->GetData()->GetFirstAbility() << " ����" << endl;
				cout << "���� ȿ�� : MP " << SearchItem->GetData()->GetSecordAbility() << " ����" << endl;
				break;
			}
			cout << endl;
		}
		SearchItem = SearchItem->GetLink();
	}

	char name[CHARSIZE];
	cout << "������ ������ �̸� : ";
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
		cout << "�̸��� �߸� �Է��߽��ϴ�." << endl;
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
		cout << "���� �����մϴ�." << endl;
	}
	
	else {
		item->MakeCode();
		cout << item->GetItemName() << " �� �����߽��ϴ�." << endl;
	}
	
	cout << NowUser->GetGameMoney() << " �� ���ҽ��ϴ�." << endl;
}

void ItemStore::Buy() {
	int sel = Menu();
	Item* item = nullptr;
	User* NowUser = LoginManager::GetInstance()->GetNowUser();
	bool flug = false;
	while (NowUser->GetItem(&item)) {
		if (item->GetKind() == sel) {
			cout << endl;
			cout << "�̸� : " << item->GetItemName() << endl;
			cout << "������ �ڵ� : " << item->GetItemCode() << endl;
			cout << "���� : " << item->GetPrice() << endl;
			cout << endl;
			flug = true;
		}
	}

	NowUser->SearchItemEnd();

	if (!flug) {
		cout << "�����ϰ� �ִ� �������� �����ϴ�." << endl;
		return;
	}

	char code[CHARSIZE];
	cout << "������ �ڵ� : ";
	cin >> code;
	flug = false;

	while (NowUser->GetItem(&item)) {
		if (!strcmp(item->GetItemCode(), code) && item->GetKind() == sel) {
			cout << item->GetItemName() << " �� �˴ϴ�." << endl;
			NowUser->Sell(item->GetItemCode());
			cout << NowUser->GetGameMoney() << " �� ���ҽ��ϴ�." << endl;
			flug = true;
			break;
		}
	}

	NowUser->SearchItemEnd();

	if (!flug) {
		cout << "������ �ڵ带 �߸� �Է��߽��ϴ�." << endl;
	}
}

int ItemStore::Menu() {
	while (1) {
		int sel;
		cout << "< ������ ���� >" << endl;
		for (int i = 1; i < sizeof(ItemName) / sizeof(char*); i++) {
			cout << i << ". " << ItemName[i] << endl;
		}
		cout << "���� : ";
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