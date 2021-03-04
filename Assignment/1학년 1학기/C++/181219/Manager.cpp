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
		cout << "< ��ǰ ���� ���α׷� >" << endl;
		cout << "1. ��ǰ �԰�" << endl;
		cout << "2. ��Ǯ ���" << endl;
		cout << "3. ��ǰ ��ȸ" << endl;
		cout << "4. ����" << endl;
		cout << "���� : ";	cin >> sel;
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
			cout << "�߸� �Է��߽��ϴ�." << endl;
			continue;
		}
		if (flug) {
			cout << "< ���� >" << endl;
			return;
		}
	}
}