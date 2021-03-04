#include "MainManager.h"

MainManager::MainManager() {
	memset(DB, 0, sizeof(DB));
}

MainManager::~MainManager() {
	for (int i = 0; i < 4; i++) {
		if (DB[i] != nullptr) {
			delete DB[i];
			DB[i] = nullptr;
		}
	}
}

void MainManager::CinCheck() {
	if (cin.failbit) {
		cin.clear();
		cin.ignore(256, '\n');
	}
}

void MainManager::init() {
	DB[PROGRAMING] = new CPrograming();
	DB[GRAPHIC] = new CGraphic();
	DB[DESIGN] = new CDesign();
}

int MainManager::Menu() {
	cout << "<���� ���α׷�>" << endl << "1. �����Է�" << endl << "2. �������"
		<< endl << "3. ������ȸ" << endl << "4. ����" << endl;
	cout << "���� : ";
	int num;
	cin >> num;
	CinCheck();
	return num;
}

void MainManager::run() {
	init();
	int sel;

	while (1) {
		switch (Menu()) {
		case INPUT:

			cout << "<�����Է�>" << endl << "<���α׷��� : 1 , �׷��� : 2 , ��ȹ : 3>" << endl;
			cout << "���� : ";
			cin >> sel;
			CinCheck();
			
			switch (sel) {
			case PROGRAMING:
				DB[PROGRAMING]->input();
				break;

			case GRAPHIC:
				DB[GRAPHIC]->input();
				break;

			case DESIGN:
				DB[DESIGN]->input();
				break;
			}
			break;

		case DISPLAY:

			cout << "<�����Է�>" << endl << "<���α׷��� : 1 , �׷��� : 2 , ��ȹ : 3>" << endl;
			cout << "���� : ";
			cin >> sel;
			CinCheck();
		
			switch (sel) {
			case PROGRAMING:
				DB[PROGRAMING]->display();
				break;

			case GRAPHIC:
				DB[GRAPHIC]->display();
				break;

			case DESIGN:
				DB[DESIGN]->display();
				break;
			}
			break;

		case SEARCH:

			cout << "<�����Է�>" << endl << "<���α׷��� : 1 , �׷��� : 2 , ��ȹ : 3>" << endl;
			cout << "���� : ";
			cin >> sel;
			CinCheck();
			
			switch (sel) {
			case PROGRAMING:
				DB[PROGRAMING]->search();
				break;

			case GRAPHIC:
				DB[GRAPHIC]->search();
				break;

			case DESIGN:
				DB[DESIGN]->search();
				break;
			}
			break;

		case EXIT:
			return;
			break;
		}
	}
}