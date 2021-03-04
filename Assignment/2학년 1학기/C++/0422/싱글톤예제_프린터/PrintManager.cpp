#include "PrintManager.h"

PrintManager::PrintManager() {
	PrintM = nullptr;
}

PrintManager::~PrintManager() {

}

int PrintManager::Menu() {
	while (1) {
		cout << "< 프린터 메뉴 >\n" << "1. 입력\n" << "2. 출력\n" << "3. 종료\n";
		cout << "선택 : ";

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