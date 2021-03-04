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

	cout << "<성적 프로그램>" << endl << "1. 성적입력" << endl << "2. 성적출력" << endl << "3. 성적조회" << endl << "4. 종료" << endl;
	cout << "입력 : "; 
	cin >> sel;
	if (cin.failbit) {
		cin.clear();
		cin.ignore(256, '\n');
	}
	return sel;
}

int CManager::SubMenu() {
	int sel;

	cout << "<1. 프로그래밍 / 2. 그래픽 / 3. 기획 >" << endl << "선택 : ";
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