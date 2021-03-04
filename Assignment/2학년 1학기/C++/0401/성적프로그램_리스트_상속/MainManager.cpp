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
	cout << "<성적 프로그램>" << endl << "1. 성적입력" << endl << "2. 성적출력"
		<< endl << "3. 성적조회" << endl << "4. 종료" << endl;
	cout << "선택 : ";
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

			cout << "<성적입력>" << endl << "<프로그래밍 : 1 , 그래픽 : 2 , 기획 : 3>" << endl;
			cout << "선택 : ";
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

			cout << "<성적입력>" << endl << "<프로그래밍 : 1 , 그래픽 : 2 , 기획 : 3>" << endl;
			cout << "선택 : ";
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

			cout << "<성적입력>" << endl << "<프로그래밍 : 1 , 그래픽 : 2 , 기획 : 3>" << endl;
			cout << "선택 : ";
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