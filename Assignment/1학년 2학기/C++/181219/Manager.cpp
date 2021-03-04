#include "Manager.h"

cMain::cMain() {
	system = new NodeManager();
}
cMain::~cMain() {
	delete system;
}

void cMain::Run() {
	while (1) {

		int sel;
		cout << "======================================================" << endl;
		cout << "< 제품 관리 프로그램 >" << endl;
		cout << "1. 제품 입고" << endl;
		cout << "2. 제풀 출고" << endl;
		cout << "3. 제품 조회" << endl;
		cout << "4. 종료" << endl;
		cout << "선택 : ";	cin >> sel;
		bool flug = false;

		switch (sel)
		{
		case INPUT:
			system->intList();
			break;

		case OUTPUT:
			system->doubleList();
			break;

		case SHOW:
			system->charList();
			break;

		case END:
			flug = true;
			break;
		default:
			cout << "잘못 입력했습니다." << endl;
			continue;
		}
		if (flug) {
			cout << "< 종료 >" << endl;
			return;
		}
	}
}