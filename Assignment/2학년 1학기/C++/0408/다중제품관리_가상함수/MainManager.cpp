#include "MainManager.h"

const char* CMainManager::ProductList[] = { "가전제품","식품","의류" };

CMainManager::CMainManager() {
	memset(Total, 0, sizeof(Total));
}

CMainManager::~CMainManager() {
	for (int i = 1; i < sizeof(ProductList) / sizeof(char*) + 1; i++) {
		if (Total[i] != nullptr) {
			delete Total[i];
			Total[i] = nullptr;
		}
	}
}

void CMainManager::init() {
	Total[ITEM] = new CItemManager();
	Total[FOOD] = new CFoodManager();
	Total[WEAR] = new CWearManager();

	for (int i = 1; i < sizeof(ProductList) / sizeof(char*) + 1; i++) {
		Total[i]->init();
	}
}

CManager* CMainManager::Menu() {
	while (1) {
		for (int i = 0; i < sizeof(ProductList) / sizeof(char*); i++) {
			cout << i + 1 << ". " << ProductList[i] << endl;
		}
		cout << "선택 : ";
		int sel;
		cin >> sel;

		if (cin.failbit) {
			cin.clear();
			cin.ignore(256, '\n');
		}

		if (sel <= NODATA || sel > sizeof(ProductList) / sizeof(char*)) {
			cout << "잘못 입력하셨습니다." << endl;
			continue;
		}

		return Total[sel];
	}
}

int CMainManager::MainMenu() {
	while (1) {
		cout << "<<\t선택\t>>" << endl << "1. 상품 추가" << endl << "2. 상품 검색" << endl << "3. 상품 삭제" << endl << "4. 종료" << endl;
		cout << "선택 : ";
		int sel;
		cin >> sel;

		if (cin.failbit) {
			cin.clear();
			cin.ignore(256, '\n');
		}

		if (sel <= NODATA || sel > 4) {
			cout << "잘못 입력하셨습니다." << endl;
			continue;
		}

		return sel;
	}
}

void CMainManager::run() {
	init();

	while (1) {
		switch (MainMenu()) {
		case INPUT:
			Menu()->Input();
			break;

		case SEARCH:
			Menu()->Search();
			break;

		case DELETE:
			Menu()->Delete();
			break;

		case EXIT:
			return;
			break;
		}
	}
}