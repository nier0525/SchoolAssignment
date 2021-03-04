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
	cout << "< �޴� >" << endl << "1. ȸ������" << endl << "2. �α���" <<
		endl << "3. ����" << endl << "���� : ";
	int sel;
	cin >> sel;
	if (cin.failbit) {
		cin.clear();
		cin.ignore(256, '\n');
	}
	return sel;
}

int Manager::LoginMenu() {
	cout << "< �α��� �޴� >" << endl << "1. �α׾ƿ�" << endl 
		<< "2. ȸ��Ż��" << endl << "���� : ";
	int sel;
	cin >> sel;
	if (cin.failbit) {
		cin.clear();
		cin.ignore(256, '\n');
	}
	return sel;
}