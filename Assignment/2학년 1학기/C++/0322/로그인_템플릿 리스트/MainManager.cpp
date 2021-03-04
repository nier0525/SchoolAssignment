#include "MainManager.h"

Manager::Manager() {
	LM = nullptr;
}

Manager::~Manager() {
	delete LM;
}

void Manager::init() {
	LM = new LoginManager();
}

void Manager::Run() {
	init();
	LM->init();
	while (1) {
		switch (MainMeun()) {
		case JOIN:
			LM->New();
			break;

		case LOGIN:
			if (LM->Login()) {
				switch (LoginMenu()) {
				case LOGOUT:
					LM->Logout();
					break;

				case DEL:
					LM->Del();
					break;
				}
			}
			break;

		case EXIT:
			return;
			break;
		}
	}
}

int Manager::MainMeun() {
	cout << "< 메뉴 >" << endl << "1. 회원가입" << endl << "2. 로그인" <<
		endl << "3. 종료" << endl << "선택 : ";
	int sel;
	cin >> sel;
	if (cin.failbit) {
		cin.clear();
		cin.ignore(256, '\n');
	}
	return sel;
}

int Manager::LoginMenu() {
	cout << "< 로그인 메뉴 >" << endl << "1. 로그아웃" << endl 
		<< "2. 회원탈퇴" << endl << "선택 : ";
	int sel;
	cin >> sel;
	if (cin.failbit) {
		cin.clear();
		cin.ignore(256, '\n');
	}
	return sel;
}