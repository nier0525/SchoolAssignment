#include "MainManager.h"

CManager::CManager() {
	memset(Total, 0, sizeof(Total));
}

CManager::~CManager() {
	for (int i = 0; i < 4; i++) {
		if (Total[i] != nullptr) {
			delete Total[i];
			Total[i] = nullptr;
		}
	}
}

void CManager::init() {
	Total[PROGRAMING] = new CPrograming();
	Total[GRAPHIC] = new CGraphic();
	Total[DESIGN] = new CDesign();
}

int CManager::Menu() {
	int sel;

	cout << "<���� ���α׷�>" << endl << "1. �����Է�" << endl << "2. �������" << endl << "3. ������ȸ" << endl << "4. ����" << endl;
	cout << "�Է� : "; 
	cin >> sel;
	if (cin.failbit) {
		cin.clear();
		cin.ignore(256, '\n');
	}
	return sel;
}

int CManager::SubMenu() {
	int sel;

	cout << "<1. ���α׷��� / 2. �׷��� / 3. ��ȹ >" << endl << "���� : ";
	cin >> sel;
	if (cin.failbit) {
		cin.clear();
		cin.ignore(256, '\n');
	}
	return sel;
}

void CManager::InputMenu() {
	switch (SubMenu()) {
	case PROGRAMING:
		Total[PROGRAMING]->Input();
		break;

	case GRAPHIC:
		Total[GRAPHIC]->Input();
		break;

	case DESIGN:
		Total[DESIGN]->Input();
		break;
	}
}

void CManager::DisplayMenu() {
	switch (SubMenu()) {
	case PROGRAMING:
		Total[PROGRAMING]->Display();
		break;

	case GRAPHIC:
		Total[GRAPHIC]->Display();
		break;

	case DESIGN:
		Total[DESIGN]->Display();
		break;
	}

}

void CManager::SearchMenu() {
	switch (SubMenu()) {
	case PROGRAMING:
		Total[PROGRAMING]->Search();
		break;

	case GRAPHIC:
		Total[GRAPHIC]->Search();
		break;

	case DESIGN:
		Total[DESIGN]->Search();
		break;
	}
}

void CManager::Run() {
	init();

	while (1) {
		switch (Menu()) {
			case INPUT:
				InputMenu();
				break;

			case DISPLAY:
				DisplayMenu();
				break;

			case SEARCH:
				SearchMenu();
				break;

			case EXIT:
				return;
				break;
		}
	}
}