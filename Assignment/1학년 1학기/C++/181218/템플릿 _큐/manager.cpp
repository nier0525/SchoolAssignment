#include "Manager.h"

cManager::cManager() {}

cManager::~cManager() {
	delete cQ;
}

int cManager::SetMenu() {
	cout << "< 큐 시스템 >" << endl;
	cout << "1. int" << endl;
	cout << "2. double" << endl;
	cout << "3. char" << endl;
	cout << "4. Exit" << endl;
	cout << "Selet : ";
	int sel;
	cin >> sel;

	return sel;
}

void cManager::run() {

	while (1) {
		
		int sel = SetMenu();
		
		switch (sel) {
		case 1:
			cQ->intQ();
			break;

		case 2:
			cQ->doubleQ();
			break;

		case 3:
			cQ->charQ();
			break;

		case 4:
			cout << "< 종료 >";
			return;

		default:
			cout << "잘못 입력하셨습니다." << endl;
			continue;
		}
	}
}