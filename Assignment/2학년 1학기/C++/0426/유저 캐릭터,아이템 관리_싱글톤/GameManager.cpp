#include "GameManager.h"

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
	CharacterManager::GetInstance()->Init();
	ItemStore::GetInstance()->Init();
}

void GameManager::Run() {
	while (1) {
		switch (GameMenu()) {
		case NEW:
			CharacterManager::GetInstance()->NewChar();
			break;

		case DEL:
			CharacterManager::GetInstance()->DelChar();
			break;

		case SELECT:
			CharacterManager::GetInstance()->SelChar();
			break;

		case BUY:
			ItemStore::GetInstance()->Sell();
			break;

		case SELL:
			ItemStore::GetInstance()->Buy();
			break;

		case LOGOUT:
			LoginManager::GetInstance()->Logout();
			return;

		case DELLOG:
			LoginManager::GetInstance()->Delete();
			return;
		}
	}
}

int GameManager::GameMenu() {
	while (1) {
		int sel;

		cout << "< ���� �޴� >\n" << "1. ĳ���� ����\n" << "2. ĳ���� ����\n" << "3. ĳ���� ����\n"
			<< "4. ������ ����\n" << "5. ������ �Ǹ�\n" << "6. �α׾ƿ�\n" << "7. ȸ��Ż��\n";
		cout << "���� : ";
		cin >> sel;
		
		if (cin.failbit) {
			cin.clear();
			cin.ignore(256, '\n');
		}
		
		if (sel < 1 || sel > 7) {
			cout << "< Error >" << endl;
			continue;
		}

		return sel;
	}
}