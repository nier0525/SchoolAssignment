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
	cout << "<성적프로그램>" << endl << "1. 성적입력" << endl << "2. 성적출력" <<
		endl << "3. 성적조회" << endl << "4. 종료" << endl;
	cout << "선택 : ";
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
			cout << "<성적입력>" << endl << "<프로그래밍:1, 그래픽:2, 기획:3>" << endl;
			cout << "입력 : ";
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
			cout << "<성적출력>" << endl << "<프로그래밍:1, 그래픽:2, 기획:3>" << endl;
			cout << "입력 : ";
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
			cout << "<성적조회>" << endl << "<프로그래밍:1, 그래픽:2, 기획:3>" << endl;
			cout << "입력 : ";
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