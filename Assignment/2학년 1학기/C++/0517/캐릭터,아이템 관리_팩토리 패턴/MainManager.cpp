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
			cout << "프로그램을 종료합니다." << endl;
			return;
			break;
		}
	}
}

int MainManager::Menu() {
	while (1) {
		int sel;

		cout << "< 메뉴 >\n" << "1. 회원가입\n" << "2. 로그인\n" << "3. 종료\n" << "선택 : ";
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