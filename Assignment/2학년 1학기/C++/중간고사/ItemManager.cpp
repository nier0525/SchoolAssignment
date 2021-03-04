#include "ItemManager.h"

// ������ �޴� �� ���� �������� ������ char �� �迭
const char* ItemManager::ItemName[] = { " ", "����" , "��" , "����" };

const char* ItemManager::PortionList[] = { " ", "HP ����" , "MP ����" };
const char* ItemManager::ArmorList[] = { " ", "����" , "����" , "����" };
const char* ItemManager::WenponList[] = { " ", "��" , "Ȱ" , "�ϵ�" };

// ������ ��� ���� �ʱ�ȭ �۾�
ItemManager::ItemManager() {
	memset(Itemlist,0,sizeof(Itemlist));
	NowItem = nullptr;
}

// �Ҹ��� �޸� ��ȯ �۾�
ItemManager::~ItemManager() {
	for (int i = 0; i < 4; i++) {
		delete Itemlist[i];
		Itemlist[i] = nullptr;
	}
	delete NowItem;
}

// �������� ���� ���� ���� �̹� ������� ��ü������ ������ ��ü�� ����� ���� ���� �Լ�
// ���� ��ü ���� ������ ����Ʈ�� ������ �Ѱ��ֱ⸸ �ϱ� ������ ����,��ȯ X
void ItemManager::Init() {
	// ����Ʈ �迭 ���� ���� �Ҵ� ( ���� �ʴ� 0 ��° �迭�� �Ҵ����� ���� )
	// �Ŀ� ������ �߰�,������ �����ϱ� ���� ������ �޴� char �迭 ������ for �� ����
	for (int i = 1; i < sizeof(ItemName) / sizeof(char*); i++) {
		Itemlist[i] = new ListNode<Item*>();
	}

	// ����
	Itemlist[PORTION]->Input(new Portion(PortionList[HP], 50));
	Itemlist[PORTION]->Input(new Portion(PortionList[MP], 50));

	// NowItem �� ����Ʈ�� ��ȸ��Ű�鼭 ����Ʈ ���� ���Ե� ��ü���� ���� ���� ����
	NowItem = Itemlist[PORTION]->GetHead();
	while (NowItem != nullptr) {
		if (!strcmp(NowItem->GetData()->GetName(), PortionList[HP])) {
			NowItem->GetData()->SetAblity("HP 100 ȸ��");
		}
		if (!strcmp(NowItem->GetData()->GetName(), PortionList[MP])) {
			NowItem->GetData()->SetAblity("HP 150 ȸ��");
		}
		NowItem = NowItem->GetNext();
	}

	// ��
	Itemlist[ARMOR]->Input(new Armor(ArmorList[HEAD], 300));
	Itemlist[ARMOR]->Input(new Armor(ArmorList[TOP], 300));
	Itemlist[ARMOR]->Input(new Armor(ArmorList[SHIELD], 500));

	// NowItem �� ����Ʈ�� ��ȸ��Ű�鼭 ����Ʈ ���� ���Ե� ��ü���� ���� ���� ����
	NowItem = Itemlist[ARMOR]->GetHead();
	while (NowItem != nullptr) {
		if (!strcmp(NowItem->GetData()->GetName(), ArmorList[HEAD])) {
			NowItem->GetData()->SetAblity("���� 30 ����");
		}
		if (!strcmp(NowItem->GetData()->GetName(), ArmorList[TOP])) {
			NowItem->GetData()->SetAblity("���� 30 ����");
		}
		if (!strcmp(NowItem->GetData()->GetName(), ArmorList[SHIELD])) {
			NowItem->GetData()->SetAblity("���� 50 ����");
		}
		NowItem = NowItem->GetNext();
	}

	// ����
	Itemlist[WENPON]->Input(new Wenpon(WenponList[SWORD], 500));
	Itemlist[WENPON]->Input(new Wenpon(WenponList[BOW], 500));
	Itemlist[WENPON]->Input(new Wenpon(WenponList[WAND], 500));

	// NowItem �� ����Ʈ�� ��ȸ��Ű�鼭 ����Ʈ ���� ���Ե� ��ü���� ���� ���� ����
	NowItem = Itemlist[WENPON]->GetHead();
	while (NowItem != nullptr) {
		if (!strcmp(NowItem->GetData()->GetName(),WenponList[SWORD])) {
			NowItem->GetData()->SetAblity("���ݷ� 100 ����");
		}
		if (!strcmp(NowItem->GetData()->GetName(), WenponList[BOW])) {
			NowItem->GetData()->SetAblity("���ݷ� 50 ����");
		}
		if (!strcmp(NowItem->GetData()->GetName(), WenponList[WAND])) {
			NowItem->GetData()->SetAblity("���� 100 ����");
		}
		NowItem = NowItem->GetNext();
	}
}

// ������ ���� �޴�
int ItemManager::Menu() {
	while (1) {
		int sel;
		cout << "< ������ ���� >" << endl;

		// ItemName[] �迭�� ����ִ� char �� ���
		for (int i = 1; i < sizeof(ItemName) / sizeof(char*); i++) {
			cout << i << ". " << ItemName[i] << endl;
		}

		cout << "���� : ";
		cin >> sel;
		// cin ���� �Է��� sel �� ����ó��
		if (cin.failbit) {
			cin.clear();
			cin.ignore(256, '\n');
		}
		// sel ���� ������ �� ������ ��� ��� ó��
		if (sel < 1 || sel > sizeof(ItemName) / sizeof(char*) - 1) {
			cout << "< Error >" << endl;
			continue;	// �ٽ� ����
		}
		return sel; // sel �� ����
	}
}

// ������ ������ ���� ���� ����
const char* ItemManager::ItemMenu(int _item) {
	switch (_item) {	// �������� ���� ��, �� ���� �°� ����ġ�� ����
	case PORTION:
		while (1) {
			int sel;
			cout << "< ���� ���� >" << endl;
			for (int i = 1; i < sizeof(PortionList) / sizeof(char*); i++) {
				cout << i << ". " << PortionList[i] << endl;
			}
			cout << "���� : ";
			cin >> sel;
			if (cin.failbit) {
				cin.clear();
				cin.ignore(256, '\n');
			}
			if (sel < 1 || sel > sizeof(PortionList) / sizeof(char*) - 1) {
				cout << "< Error >" << endl;
				continue;
			}
			return PortionList[sel];	// PortionList �迭 ������ sel ��° char �� ����
		}
		break;
		// ���� �Ʒ� ���� ������ �۾�
	case ARMOR:
		while (1) {
			int sel;
			cout << "< �� ���� >" << endl;
			for (int i = 1; i < sizeof(ArmorList) / sizeof(char*); i++) {
				cout << i << ". " << ArmorList[i] << endl;
			}
			cout << "���� : ";
			cin >> sel;
			if (cin.failbit) {
				cin.clear();
				cin.ignore(256, '\n');
			}
			if (sel < 1 || sel > sizeof(ArmorList) / sizeof(char*) - 1) {
				cout << "< Error >" << endl;
				continue;
			}
			return ArmorList[sel];
		}
		break;

	case WENPON:
		while (1) {
			int sel;
			cout << "< ���� ���� >" << endl;
			for (int i = 1; i < sizeof(WenponList) / sizeof(char*); i++) {
				cout << i << ". " << WenponList[i] << endl;
			}
			cout << "���� : ";
			cin >> sel;
			if (cin.failbit) {
				cin.clear();
				cin.ignore(256, '\n');
			}
			if (sel < 1 || sel > sizeof(WenponList) / sizeof(char*) - 1) {
				cout << "< Error >" << endl;
				continue;
			}
			return WenponList[sel];
		}
		break;
	}
}

void ItemManager::Buy(Node<CUser*>* NowUser) {
	char select[CHARSIZE];
	int  index;
	int  count, money;
	index = Menu();	// ������ ���� �� ����
	// ItemMenu ���� ���� ���� char ���� select �� ����
	strcpy(select, ItemMenu(index));

	// ���� ���� �����۸� ��ȸ�� ����� ��ġ�� ������ ����Ʈ ��� ����
	NowUser->GetData()->SetItem(NowUser->GetData()->GetItemhead());

	while (NowUser->GetData()->GetItem() != nullptr) { // ��尡 ��ȸ�ϸ鼭 nullptr �� ����������
		// ������ ������ ����Ʈ�� ������ �������� �����Ѵٸ�
		if (!strcmp(NowUser->GetData()->GetItem()->GetData()->GetName(), select)) {
			cout << "���� : ";
			cin >> count;
			// cin ���� �Է��� count �� ����ó��
			if (cin.failbit) {
				cin.clear();
				cin.ignore(256, '\n');
			}
			// money = �������� ���� * �����ϴ� ������ ����
			money = NowUser->GetData()->GetItem()->GetData()->GetMoney() * count;
			// ������ ���� ���� �������� ��⿡ ����ϴٸ� true
			if (NowUser->GetData()->CheakMoney(money)) {
				// ������ ���� �ش� �������� ������ �ø��� �۾�
				// count = ������ ������ ���� + ������ ������ ������ �ִ� ������ ����
				count += NowUser->GetData()->GetItem()->GetData()->GetCount();
				NowUser->GetData()->GetItem()->GetData()->SetCount(count);
				// money = ������ ������ �ִµ� - money(�� �� money �� ���� ������ ���� * �����ϴ� ������ ����)
				// ������ �������� �� ���� ��ŭ �ڽ��� ���� ���� �Ҵ´�.
				money = NowUser->GetData()->Getmoney() - money;
				NowUser->GetData()->Setmoney(money);
				break; // while ����
			}
			// ���� ������ ���� ���� �������� ��⿡ ������� �ʴٸ�
			else {
				cout << "���� �����մϴ�." << endl;
				return; // �Լ� ����
			}
		}
		NowUser->GetData()->SetItem(NowUser->GetData()->GetItem()->GetNext());
		// ���� ���� �̵�
	}

	// ������ �������� ó�� �����ϴ� ���
	// ������ ������ ����Ʈ�� ������ ��ȸ�������� ������ �������� �߰����� ���� ���
	if (NowUser->GetData()->GetItem() == nullptr) {
		Item* ptr;	// ������ ��ü
		// ������ ������ ǰ���� ����Ʈ�� ��ȸ�� ������ ����� ��ġ ����
		NowItem = Itemlist[index]->GetHead();
		while (NowItem != nullptr) { // ��ȸ�ϴ� ��尡 nullptr �� ����������
			// ����Ʈ ���� ������ ������ �������� ã���� ���
			if (!strcmp(NowItem->GetData()->GetName(),select)) {
				break; // while ����
			}
			NowItem = NowItem->GetNext();
			// ���� ���� �̵�
		}
		// ��ü �����Ϳ� ���� ��尡 ����Ű�� ����� ��ü�� ����
		ptr = NowItem->GetData();

		cout << "���� : ";
		cin >> count;
		// cin ���� �Է��� count �� ����ó��
		if (cin.failbit) {
			cin.clear();
			cin.ignore(256, '\n');
		}
		// money = �������� ���� * ������ ������ ����
		money = ptr->GetMoney() * count;
		// ������ ���� ���� �������� ��⿡ ����ϴٸ� true
		if (NowUser->GetData()->CheakMoney(money)) {
			// ������ ������ ���� ����
			ptr->SetCount(count);
			// ������ ������ ����Ʈ�� ��ü�� ������ ����
			NowUser->GetData()->Buy(ptr);
			// money = ���� ������ ���� �� - money (������ ���� *  �����ϴ� ������ ����)
			money = NowUser->GetData()->Getmoney() - money;
			// ������ money ���� ����
			NowUser->GetData()->Setmoney(money);
		}
		// ���� ������ ���� ���� �������� ��⿡ ������� �ʴٸ�
		else {
			cout << "���� �����մϴ�." << endl;
			return; // �Լ� ����
		}
	}

	// ������ ���� ������ ǰ��� �� ǰ�� ���� ������ �ִ� ����, �� �������� ����
	// �׸��� ������ ���� ������ �ִ� ���� �Ӹ��� ����Ѵ�.
	NowUser->GetData()->SetItem(NowUser->GetData()->GetItemhead());
	while (NowUser->GetData()->GetItem() != nullptr) {
		if (!strcmp(NowUser->GetData()->GetItem()->GetData()->GetName(), select)) {
			cout << "���� �÷��̾ ������ " << select << " ������ "
				<< NowUser->GetData()->GetItem()->GetData()->GetCount() << " ���Դϴ�. " << endl;
			cout << select << "�� ������ " << NowUser->GetData()->GetItem()->GetData()->GetAblity() << " �Դϴ�." << endl;
			cout << "���� ���ӸӴ� : " << NowUser->GetData()->Getmoney() << " ��" << endl;
			return;
		}
	NowUser->GetData()->SetItem(NowUser->GetData()->GetItem()->GetNext());
	}
}

void ItemManager::Sell(Node<CUser*>* NowUser) {
	char select[CHARSIZE];
	int index;
	index = Menu();	// ������ ���� �� ����
	// ItemMenu ���� ���� ���� char ���� select �� ����
	strcpy(select, ItemMenu(index));

	// ���� ���� �����۸� ��ȸ�� ����� ��ġ�� ������ ����Ʈ ��� ����
	NowUser->GetData()->SetItem(NowUser->GetData()->GetItemhead());

	while (NowUser->GetData()->GetItem() != nullptr) {  // ��尡 ��ȸ�ϸ鼭 nullptr �� ����������
		// ������ ������ ����Ʈ�� ������ �������� �����Ѵٸ�
		if (!strcmp(NowUser->GetData()->GetItem()->GetData()->GetName(), select)) {
			int count, money;
			cout << "���� : ";
			cin >> count;
			// cin ���� �Է��� count �� ����ó��
			if (cin.failbit) {
				cin.clear();
				cin.ignore(256, '\n');
			}
			// ������ ������ �ִ� ������ ���� - �Ǹ��� ������ ������ 0 ���� ���� ���
			if (NowUser->GetData()->GetItem()->GetData()->GetCount() - count <= NODATA) {
				// count �� ���� ������ ������ �ִ� �������� ������ ��
				count = NowUser->GetData()->GetItem()->GetData()->GetCount();
				// money = ������ ���� �� + �������� ���� * count(���� ������ ���� ������ ����)
				money = NowUser->GetData()->Getmoney() + NowUser->GetData()->GetItem()->GetData()->GetMoney() * count;
				// ������ money ���� ����
				NowUser->GetData()->Setmoney(money);
				// ������ ������ ����Ʈ���� �ش� ������ ��� ���� ( ���� ������ 0 �� )
				NowUser->GetData()->Sell(NowUser->GetData()->GetItem());
			}
			// ������ ���� �������� ������ �Ǹ��Ŀ��� �����ִٸ�
			else {
				// temp = ���� ������ ���� ������ ���� - �Ǹ��� ������ ����
				int temp = NowUser->GetData()->GetItem()->GetData()->GetCount() - count;
				// money = ������ ���� �� + ������ ���� * count
				money = NowUser->GetData()->Getmoney() + NowUser->GetData()->GetItem()->GetData()->GetMoney() * count;
				// ���� ������ ���� �������� ������ temp �� �ȴ�.
				NowUser->GetData()->GetItem()->GetData()->SetCount(temp);
				// ���� ������ ���� money �� �ȴ�.
				NowUser->GetData()->Setmoney(money);
			}

			cout << select << "��(��) " << count << " �� �Ⱦҽ��ϴ�." << endl;
			cout << "���� ���� ���ӸӴϴ� " << NowUser->GetData()->Getmoney() << " ���Դϴ�." << endl;
			return; // �Լ� ����
		}
		NowUser->GetData()->SetItem(NowUser->GetData()->GetItem()->GetNext());
		// ���� ���� �̵�
	}
	// ���� ������ ��ȸ�������� �ش� �������� ���� ������ ����Ʈ���� ã�� ���� ���
	if (NowUser->GetData()->GetItem() == nullptr) {
		cout << "������ �������� ���� ���� �κ��丮�� �����ϴ�." << endl;
		return; // �Լ� ����
	}
}