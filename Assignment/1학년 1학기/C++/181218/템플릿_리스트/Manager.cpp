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
		cout << "< 메뉴 >" << endl;
		cout << "1. int 형 리스트" << endl;
		cout << "2. double 형 리스트" << endl;
		cout << "3. char 형 리스트" << endl;
		cout << "4. 종료" << endl;
		cout << "선택 : ";	cin >> sel;
		bool flug = false;

		switch (sel)
		{
		case INT:
			system->intList();
			break;

		case DOUBLE:
			system->doubleList();
			break;

		case CHAR:
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