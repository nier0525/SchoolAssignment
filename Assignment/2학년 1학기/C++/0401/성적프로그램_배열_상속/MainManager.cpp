#include "MainManager.h"

MainManager::MainManager() {
	Programing = nullptr;
	Graphic = nullptr;
	Producter = nullptr;
}

MainManager::~MainManager() {
	delete Programing, Graphic, Producter;
}

int MainManager::MainMenu() {
	cout << "<�������α׷�>" << endl << "1. �����Է�" << endl << "2. �������" <<
		endl << "3. ������ȸ" << endl << "4. ����" << endl;
	cout << "���� : ";
	int sel;
	cin >> sel;
	CinCheak();
	return sel;
}

void MainManager::CinCheak() {
	if (cin.failbit) {
		cin.clear();
		cin.ignore(256, '\n');
	}
}

void MainManager::init() {
	Programing = new CPrograming();
	Graphic = new CGraphic();
	Producter = new CProducter();
}

void MainManager::run() {
	init();

	while (1) {
		switch (MainMenu())
		{
		case INPUT:
			cout << "<�����Է�>" << endl << "<���α׷���:1, �׷���:2, ��ȹ:3>" << endl;
			cout << "�Է� : ";
			int sel;
			cin >> sel;
			CinCheak();

			switch (sel) {
			case PROGRAMING:
				Programing->input();
				break;

			case GRAPHIC:
				Graphic->input();
				break;

			case PRODUCTER:
				Producter->input();
				break;
			}
			break;

		case DISPLAY:
			cout << "<�������>" << endl << "<���α׷���:1, �׷���:2, ��ȹ:3>" << endl;
			cout << "�Է� : ";
			int sel2;
			cin >> sel2;
			CinCheak();

			switch (sel2) {
			case PROGRAMING:
				Programing->display();
				break;

			case GRAPHIC:
				Graphic->display();
				break;

			case PRODUCTER:
				Producter->display();
				break;
			}
			break;

		case SEARCH:
			cout << "<������ȸ>" << endl << "<���α׷���:1, �׷���:2, ��ȹ:3>" << endl;
			cout << "�Է� : ";
			int sel3;
			cin >> sel3;
			CinCheak();

			switch (sel3) {
			case PROGRAMING:
				Programing->serach();
				break;

			case GRAPHIC:
				Graphic->serach();
				break;

			case PRODUCTER:
				Producter->serach();
				break;
			}
			break;

		case EXIT:
			return;
			break;
		}
	}
}