#include "MainManager.h"
#include "GameManager.h"

MainManager* MainManager::pthis = nullptr;

MainManager* MainManager::GetInstance() {
	if (!pthis) {
		pthis = new MainManager();
	}
	LoginManager::GetInstance();
	GameManager::GetInstance();

	return pthis;
}

void MainManager::Destory() {
	if (pthis) {
		delete pthis;
		pthis = nullptr;
	}
	LoginManager::Destory();
	GameManager::Destory();
}

MainManager::MainManager() {

}

MainManager::~MainManager() {

}

void MainManager::Init() {
	LoginManager::GetInstance()->Init();
	GameManager::GetInstance()->Init();
}

void MainManager::Run() {
	while (1) {
		switch (Menu()) {
		case NEW:
			LoginManager::GetInstance()->New();
			break;

		case LOGIN:
			if (LoginManager::GetInstance()->Login()) {
				GameManager::GetInstance()->Run();
			}
			break;

		case EXIT:
			cout << "���α׷��� �����մϴ�." << endl;
			return;
			break;
		}
	}
}

int MainManager::Menu() {
	while (1) {
		int sel;

		cout << "< �޴� >\n" << "1. ȸ������\n" << "2. �α���\n" << "3. ����\n" << "���� : ";
		cin >> sel;
		
		if (cin.failbit) {
			cin.clear();
			cin.ignore(256, '\n');
		}

		if (sel < 1 || sel > 3) {
			cout << "< Error >" << endl;
			continue;
		}

		return sel;
	}
}