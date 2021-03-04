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
		cout << "< �޴� >" << endl;
		cout << "1. int �� ����Ʈ" << endl;
		cout << "2. double �� ����Ʈ" << endl;
		cout << "3. char �� ����Ʈ" << endl;
		cout << "4. ����" << endl;
		cout << "���� : ";	cin >> sel;
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
			cout << "�߸� �Է��߽��ϴ�." << endl;
			continue;
		}
		if (flug) {
			cout << "< ���� >" << endl;
			return;
		}
	}
}