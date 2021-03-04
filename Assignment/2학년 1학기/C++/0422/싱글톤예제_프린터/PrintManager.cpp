#include "PrintManager.h"

PrintManager::PrintManager() {
	PrintM = nullptr;
}

PrintManager::~PrintManager() {

}

int PrintManager::Menu() {
	while (1) {
		cout << "< ������ �޴� >\n" << "1. �Է�\n" << "2. ���\n" << "3. ����\n";
		cout << "���� : ";

		int sel;
		cin >> sel;

		if (cin.failbit) {
			cin.clear();
			cin.ignore(256, '\n');
		}

		if (sel < 1 || sel > 3) {
			cout << "< Error >\n";
			continue;
		}
		return sel;
	}
}

void PrintManager::Run() {
	while (1) {
		switch (Menu()) {
		case INPUT:
			char write[CHARSIZE];
			cout << "Write : ";	cin >> write;
			PrintM->Getdata()->SetWrite(write);
			break;

		case OUTPUT:
			cout << "Read : " << PrintM->Getdata()->GetWrite() << endl;
			break;

		case EXIT:
			PrintM->Deletedata();
			return;
			break;
		}
	}
}