#include "MainManager.h"

const char* CMainManager::ProductList[] = { "������ǰ","��ǰ","�Ƿ�" };

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
		cout << "���� : ";
		int sel;
		cin >> sel;

		if (cin.failbit) {
			cin.clear();
			cin.ignore(256, '\n');
		}

		if (sel <= NODATA || sel > sizeof(ProductList) / sizeof(char*)) {
			cout << "�߸� �Է��ϼ̽��ϴ�." << endl;
			continue;
		}

		return Total[sel];
	}
}

int CMainManager::MainMenu() {
	while (1) {
		cout << "<<\t����\t>>" << endl << "1. ��ǰ �߰�" << endl << "2. ��ǰ �˻�" << endl << "3. ��ǰ ����" << endl << "4. ����" << endl;
		cout << "���� : ";
		int sel;
		cin >> sel;

		if (cin.failbit) {
			cin.clear();
			cin.ignore(256, '\n');
		}

		if (sel <= NODATA || sel > 4) {
			cout << "�߸� �Է��ϼ̽��ϴ�." << endl;
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