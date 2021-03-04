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
		cout << "< ĳ���� �޴� >" << endl;
		cout << "1. ĳ���� ����\n" << "2. ĳ���� ����\n" << "3. ĳ���� ����\n" << "4. �α׾ƿ�\n" << "5. ȸ��Ż��\n";
		cout << "���� : ";
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
		cout << "< ĳ���� �޴� >" << endl;
		cout << "1. ������ ����\n" << "2. ������ �Ǹ�\n" << "3. ���� �޴�\n";
		cout << "���� : ";
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