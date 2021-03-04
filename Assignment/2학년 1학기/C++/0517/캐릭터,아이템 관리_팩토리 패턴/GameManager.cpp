#include "GameManager.h"
#include "LoginManager.h"

GameManager* GameManager::pthis = nullptr;

GameManager* GameManager::GetInstance() {
	if (!pthis) {
		pthis = new GameManager();
	}
	CharacterManager::GetInstance();
	ItemStore::GetInstance();

	return pthis;
}

void GameManager::Destory() {
	if (pthis) {
		delete pthis;
		pthis = nullptr;
	}
	CharacterManager::Destory();
	ItemStore::Destory();
}

GameManager::GameManager() {

}

GameManager::~GameManager() {

}

void GameManager::Init() {

}

int GameManager::CharacterMenu() {
	while (1) {
		int sel;
		cout << "< 캐릭터 메뉴 >" << endl;
		cout << "1. 캐릭터 생성\n" << "2. 캐릭터 선택\n" << "3. 캐릭터 삭제\n" << "4. 로그아웃\n" << "5. 회원탈퇴\n";
		cout << "선택 : ";
		cin >> sel;

		if (cin.failbit) {
			cin.clear();
			cin.ignore(256, '\n');
		}

		if (sel < 1 || sel > 5) {
			cout << "< Error >" << endl;
			continue;
		}

		return sel;
	}
}

int GameManager::ItemMenu() {
	while (1) {
		int sel;
		cout << "< 캐릭터 메뉴 >" << endl;
		cout << "1. 아이템 구매\n" << "2. 아이템 판매\n" << "3. 이전 메뉴\n";
		cout << "선택 : ";
		cin >> sel;

		if (cin.failbit) {
			cin.clear();
			cin.ignore(256, '\n');
		}

		if (sel < 1 || sel > 3) {
			cout << "< Error >" << endl;
			continue;
		}

		return sel;
	}
}

void GameManager::Run() {
	while (1) {
		switch (this->CharacterMenu()) {
		case NEW_C:
			CharacterManager::GetInstance()->New();
			break;

		case SEL_C:
			if (CharacterManager::GetInstance()->SelectCharacter()) {
				while (1) {
					bool flug = false;
					switch (this->ItemMenu()) {
					case BUY:
						ItemStore::GetInstance()->Buy();
						break;

					case SELL:
						ItemStore::GetInstance()->Sell();
						break;

					case PREV:
						flug = true;
						break;
					}
					if (flug) {
						break;
					}
				}
			}
			break;

		case DEL_C:
			CharacterManager::GetInstance()->Delete();
			break;

		case LOGOUT:
			LoginManager::GetInstance()->LogOut();
			return;

		case DELETE:
			LoginManager::GetInstance()->Delete();
			return;
		}
	}
}