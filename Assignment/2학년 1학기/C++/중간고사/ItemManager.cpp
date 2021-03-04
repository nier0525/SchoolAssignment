#include "ItemManager.h"

// 아이템 메뉴 및 각각 아이템의 종류의 char 값 배열
const char* ItemManager::ItemName[] = { " ", "포션" , "방어구" , "무기" };

const char* ItemManager::PortionList[] = { " ", "HP 포션" , "MP 포션" };
const char* ItemManager::ArmorList[] = { " ", "투구" , "갑옷" , "방패" };
const char* ItemManager::WenponList[] = { " ", "검" , "활" , "완드" };

// 생성자 멤버 변수 초기화 작업
ItemManager::ItemManager() {
	memset(Itemlist,0,sizeof(Itemlist));
	NowItem = nullptr;
}

// 소멸자 메모리 반환 작업
ItemManager::~ItemManager() {
	for (int i = 0; i < 4; i++) {
		delete Itemlist[i];
		Itemlist[i] = nullptr;
	}
	delete NowItem;
}

// 아이템은 기존 게임 내에 이미 만들어진 객체임으로 사전에 객체를 만들어 놓기 위한 함수
// 유저 객체 내의 아이템 리스트에 정보를 넘겨주기만 하기 때문에 생성,반환 X
void ItemManager::Init() {
	// 리스트 배열 전부 동적 할당 ( 쓰지 않는 0 번째 배열은 할당하지 않음 )
	// 후에 아이템 추가,삭제가 용이하기 위해 아이템 메뉴 char 배열 값으로 for 문 실행
	for (int i = 1; i < sizeof(ItemName) / sizeof(char*); i++) {
		Itemlist[i] = new ListNode<Item*>();
	}

	// 포션
	Itemlist[PORTION]->Input(new Portion(PortionList[HP], 50));
	Itemlist[PORTION]->Input(new Portion(PortionList[MP], 50));

	// NowItem 로 리스트를 순회시키면서 리스트 내에 삽입된 객체들의 각각 성능 설정
	NowItem = Itemlist[PORTION]->GetHead();
	while (NowItem != nullptr) {
		if (!strcmp(NowItem->GetData()->GetName(), PortionList[HP])) {
			NowItem->GetData()->SetAblity("HP 100 회복");
		}
		if (!strcmp(NowItem->GetData()->GetName(), PortionList[MP])) {
			NowItem->GetData()->SetAblity("HP 150 회복");
		}
		NowItem = NowItem->GetNext();
	}

	// 방어구
	Itemlist[ARMOR]->Input(new Armor(ArmorList[HEAD], 300));
	Itemlist[ARMOR]->Input(new Armor(ArmorList[TOP], 300));
	Itemlist[ARMOR]->Input(new Armor(ArmorList[SHIELD], 500));

	// NowItem 로 리스트를 순회시키면서 리스트 내에 삽입된 객체들의 각각 성능 설정
	NowItem = Itemlist[ARMOR]->GetHead();
	while (NowItem != nullptr) {
		if (!strcmp(NowItem->GetData()->GetName(), ArmorList[HEAD])) {
			NowItem->GetData()->SetAblity("방어력 30 증가");
		}
		if (!strcmp(NowItem->GetData()->GetName(), ArmorList[TOP])) {
			NowItem->GetData()->SetAblity("방어력 30 증가");
		}
		if (!strcmp(NowItem->GetData()->GetName(), ArmorList[SHIELD])) {
			NowItem->GetData()->SetAblity("방어력 50 증가");
		}
		NowItem = NowItem->GetNext();
	}

	// 무기
	Itemlist[WENPON]->Input(new Wenpon(WenponList[SWORD], 500));
	Itemlist[WENPON]->Input(new Wenpon(WenponList[BOW], 500));
	Itemlist[WENPON]->Input(new Wenpon(WenponList[WAND], 500));

	// NowItem 로 리스트를 순회시키면서 리스트 내에 삽입된 객체들의 각각 성능 설정
	NowItem = Itemlist[WENPON]->GetHead();
	while (NowItem != nullptr) {
		if (!strcmp(NowItem->GetData()->GetName(),WenponList[SWORD])) {
			NowItem->GetData()->SetAblity("공격력 100 증가");
		}
		if (!strcmp(NowItem->GetData()->GetName(), WenponList[BOW])) {
			NowItem->GetData()->SetAblity("공격력 50 증가");
		}
		if (!strcmp(NowItem->GetData()->GetName(), WenponList[WAND])) {
			NowItem->GetData()->SetAblity("마력 100 증가");
		}
		NowItem = NowItem->GetNext();
	}
}

// 아이템 선택 메뉴
int ItemManager::Menu() {
	while (1) {
		int sel;
		cout << "< 아이템 선택 >" << endl;

		// ItemName[] 배열에 들어있는 char 값 출력
		for (int i = 1; i < sizeof(ItemName) / sizeof(char*); i++) {
			cout << i << ". " << ItemName[i] << endl;
		}

		cout << "선택 : ";
		cin >> sel;
		// cin 으로 입력한 sel 값 예외처리
		if (cin.failbit) {
			cin.clear();
			cin.ignore(256, '\n');
		}
		// sel 값이 지정된 값 범위를 벗어날 경우 처리
		if (sel < 1 || sel > sizeof(ItemName) / sizeof(char*) - 1) {
			cout << "< Error >" << endl;
			continue;	// 다시 실행
		}
		return sel; // sel 값 리턴
	}
}

// 선택한 아이템 내의 종류 선택
const char* ItemManager::ItemMenu(int _item) {
	switch (_item) {	// 아이템을 선택 후, 그 값에 맞게 스위치문 실행
	case PORTION:
		while (1) {
			int sel;
			cout << "< 포션 상점 >" << endl;
			for (int i = 1; i < sizeof(PortionList) / sizeof(char*); i++) {
				cout << i << ". " << PortionList[i] << endl;
			}
			cout << "선택 : ";
			cin >> sel;
			if (cin.failbit) {
				cin.clear();
				cin.ignore(256, '\n');
			}
			if (sel < 1 || sel > sizeof(PortionList) / sizeof(char*) - 1) {
				cout << "< Error >" << endl;
				continue;
			}
			return PortionList[sel];	// PortionList 배열 내에서 sel 번째 char 값 리턴
		}
		break;
		// 이하 아래 역시 동일한 작업
	case ARMOR:
		while (1) {
			int sel;
			cout << "< 방어구 상점 >" << endl;
			for (int i = 1; i < sizeof(ArmorList) / sizeof(char*); i++) {
				cout << i << ". " << ArmorList[i] << endl;
			}
			cout << "선택 : ";
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
			cout << "< 무기 상점 >" << endl;
			for (int i = 1; i < sizeof(WenponList) / sizeof(char*); i++) {
				cout << i << ". " << WenponList[i] << endl;
			}
			cout << "선택 : ";
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
	index = Menu();	// 아이템 선택 값 저장
	// ItemMenu 에서 리턴 받은 char 값을 select 에 복사
	strcpy(select, ItemMenu(index));

	// 유저 내에 아이템를 순회할 노드의 위치를 아이템 리스트 헤드 노드로
	NowUser->GetData()->SetItem(NowUser->GetData()->GetItemhead());

	while (NowUser->GetData()->GetItem() != nullptr) { // 노드가 순회하면서 nullptr 를 만날때까지
		// 유저의 아이템 리스트에 구입할 아이템이 존재한다면
		if (!strcmp(NowUser->GetData()->GetItem()->GetData()->GetName(), select)) {
			cout << "개수 : ";
			cin >> count;
			// cin 으로 입력한 count 값 예외처리
			if (cin.failbit) {
				cin.clear();
				cin.ignore(256, '\n');
			}
			// money = 아이템의 가격 * 구입하는 아이템 개수
			money = NowUser->GetData()->GetItem()->GetData()->GetMoney() * count;
			// 유저가 가진 돈이 아이템을 사기에 충분하다면 true
			if (NowUser->GetData()->CheakMoney(money)) {
				// 유저가 가진 해당 아이템의 개수를 늘리는 작업
				// count = 구입할 아이템 개수 + 유저가 기존에 가지고 있던 아이템 개수
				count += NowUser->GetData()->GetItem()->GetData()->GetCount();
				NowUser->GetData()->GetItem()->GetData()->SetCount(count);
				// money = 유저가 가지고 있는돈 - money(이 때 money 의 값은 아이템 가격 * 구입하는 아이템 개수)
				// 유저는 아이템을 산 개수 만큼 자신이 가진 돈을 잃는다.
				money = NowUser->GetData()->Getmoney() - money;
				NowUser->GetData()->Setmoney(money);
				break; // while 종료
			}
			// 만일 유저가 가진 돈이 아이템을 사기에 충분하지 않다면
			else {
				cout << "돈이 부족합니다." << endl;
				return; // 함수 종료
			}
		}
		NowUser->GetData()->SetItem(NowUser->GetData()->GetItem()->GetNext());
		// 다음 노드로 이동
	}

	// 유저가 아이템을 처음 구입하는 경우
	// 유저의 아이템 리스트를 끝까지 순회했음에도 선택한 아이템을 발견하지 못한 경우
	if (NowUser->GetData()->GetItem() == nullptr) {
		Item* ptr;	// 포인터 객체
		// 유저가 선택한 품목의 리스트를 순회할 아이템 노드의 위치 설정
		NowItem = Itemlist[index]->GetHead();
		while (NowItem != nullptr) { // 순회하는 노드가 nullptr 를 만날때까지
			// 리스트 내에 유저가 선택한 아이템을 찾았을 경우
			if (!strcmp(NowItem->GetData()->GetName(),select)) {
				break; // while 중지
			}
			NowItem = NowItem->GetNext();
			// 다음 노드로 이동
		}
		// 객체 포인터에 현재 노드가 가르키는 노드의 객체를 대입
		ptr = NowItem->GetData();

		cout << "개수 : ";
		cin >> count;
		// cin 으로 입력한 count 값 예외처리
		if (cin.failbit) {
			cin.clear();
			cin.ignore(256, '\n');
		}
		// money = 아이템의 가격 * 구입할 아이템 개수
		money = ptr->GetMoney() * count;
		// 유저가 가진 돈이 아이템을 사기에 충분하다면 true
		if (NowUser->GetData()->CheakMoney(money)) {
			// 선택한 아이템 개수 대입
			ptr->SetCount(count);
			// 유저의 아이템 리스트에 객체의 정보를 삽입
			NowUser->GetData()->Buy(ptr);
			// money = 현재 유저가 가진 돈 - money (아이템 가격 *  구입하는 아이템 개수)
			money = NowUser->GetData()->Getmoney() - money;
			// 설정한 money 값을 대입
			NowUser->GetData()->Setmoney(money);
		}
		// 만일 유저가 가진 돈이 아이템을 사기에 충분하지 않다면
		else {
			cout << "돈이 부족합니다." << endl;
			return; // 함수 종료
		}
	}

	// 유저가 현재 구입한 품목과 그 품목에 대해 가지고 있는 개수, 그 아이템의 성능
	// 그리고 유저가 현재 가지고 있는 게임 머리를 출력한다.
	NowUser->GetData()->SetItem(NowUser->GetData()->GetItemhead());
	while (NowUser->GetData()->GetItem() != nullptr) {
		if (!strcmp(NowUser->GetData()->GetItem()->GetData()->GetName(), select)) {
			cout << "현재 플레이어가 보유한 " << select << " 개수는 "
				<< NowUser->GetData()->GetItem()->GetData()->GetCount() << " 개입니다. " << endl;
			cout << select << "의 성능은 " << NowUser->GetData()->GetItem()->GetData()->GetAblity() << " 입니다." << endl;
			cout << "남은 게임머니 : " << NowUser->GetData()->Getmoney() << " 원" << endl;
			return;
		}
	NowUser->GetData()->SetItem(NowUser->GetData()->GetItem()->GetNext());
	}
}

void ItemManager::Sell(Node<CUser*>* NowUser) {
	char select[CHARSIZE];
	int index;
	index = Menu();	// 아이템 선택 값 저장
	// ItemMenu 에서 리턴 받은 char 값을 select 에 복사
	strcpy(select, ItemMenu(index));

	// 유저 내에 아이템를 순회할 노드의 위치를 아이템 리스트 헤드 노드로
	NowUser->GetData()->SetItem(NowUser->GetData()->GetItemhead());

	while (NowUser->GetData()->GetItem() != nullptr) {  // 노드가 순회하면서 nullptr 를 만날때까지
		// 유저의 아이템 리스트에 삭제할 아이템이 존재한다면
		if (!strcmp(NowUser->GetData()->GetItem()->GetData()->GetName(), select)) {
			int count, money;
			cout << "개수 : ";
			cin >> count;
			// cin 으로 입력한 count 값 예외처리
			if (cin.failbit) {
				cin.clear();
				cin.ignore(256, '\n');
			}
			// 유저가 가지고 있는 아이템 개수 - 판매할 아이템 개수가 0 보다 작을 경우
			if (NowUser->GetData()->GetItem()->GetData()->GetCount() - count <= NODATA) {
				// count 는 현재 유저가 가지고 있는 아이템의 개수가 됨
				count = NowUser->GetData()->GetItem()->GetData()->GetCount();
				// money = 유저의 현재 돈 + 아이템의 가격 * count(현재 유저가 가진 아이템 개수)
				money = NowUser->GetData()->Getmoney() + NowUser->GetData()->GetItem()->GetData()->GetMoney() * count;
				// 설정한 money 값을 대입
				NowUser->GetData()->Setmoney(money);
				// 유저의 아이템 리스트에서 해당 아이템 노드 삭제 ( 가진 개수가 0 임 )
				NowUser->GetData()->Sell(NowUser->GetData()->GetItem());
			}
			// 유저가 가진 아이템의 개수가 판매후에도 남아있다면
			else {
				// temp = 현재 유저가 가진 아이템 개수 - 판매할 아이템 개수
				int temp = NowUser->GetData()->GetItem()->GetData()->GetCount() - count;
				// money = 유저의 현재 돈 + 아이템 가격 * count
				money = NowUser->GetData()->Getmoney() + NowUser->GetData()->GetItem()->GetData()->GetMoney() * count;
				// 현재 유저가 가진 아이템의 개수는 temp 가 된다.
				NowUser->GetData()->GetItem()->GetData()->SetCount(temp);
				// 현재 유저의 돈은 money 가 된다.
				NowUser->GetData()->Setmoney(money);
			}

			cout << select << "을(를) " << count << " 개 팔았습니다." << endl;
			cout << "현재 남은 게임머니는 " << NowUser->GetData()->Getmoney() << " 원입니다." << endl;
			return; // 함수 종료
		}
		NowUser->GetData()->SetItem(NowUser->GetData()->GetItem()->GetNext());
		// 다음 노드로 이동
	}
	// 만일 끝까지 순회했음에도 해당 아이템을 유저 아이템 리스트에서 찾지 못한 경우
	if (NowUser->GetData()->GetItem() == nullptr) {
		cout << "선택한 아이템이 현재 유저 인벤토리에 없습니다." << endl;
		return; // 함수 종료
	}
}