#include "MainManager.h"

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
	Init();
	while (1) {
		switch (LoginMenu())
		{
		case NEWLOG:
			LoginManager::GetInstance()->New();
			break;

		case LOGIN:
			if (LoginManager::GetInstance()->Login()) {
				GameManager::GetInstance()->Run();
			};
			break;

		case EXIT:
			return;
			break;
		}
	}
}

int MainManager::LoginMenu() {
	while (1) {
		int sel;

		cout << "< 로그인 메뉴 >\n" << "1. 회원가입\n" << "2. 로그인\n" << "3. 종료\n";
		cout << "선택 : ";

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