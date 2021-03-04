#include "Manager.h"

Manager::Manager() {
	LOG = nullptr;
	GM = nullptr;
}

Manager::~Manager() {
	delete LOG, GM;
}

void Manager::init() {
	GM = new GManager();
	LOG = new CLogin();
}

void Manager::Run() {

	init();

	while (1) {
		int sel;
		bool flug = false;

		cout << "<MainMenu>" << endl << "1. 회원가입" << endl << "2. 로그인" << endl << "3. 종료" << endl << "선택 : ";
		cin >> sel;

		switch (sel) {
		case JOIN:
			LOG->New();
			break;

		case LOGIN:
			if (LOG->Login()) {
				while (1) {

					cout << "<LoginMenu>" << endl << "1. 게임시작 " << endl << "2. 기록확인 " << endl << "3. 로그아웃" << endl << "4. 회원탈퇴" << endl << "선택 : ";
					cin >> sel;
					switch (sel) {

					case GAMESTART:
						GM->Gameinit(LOG->GetNowUser());
						GM->GameRun();
						GM->GameEnd();
						continue;
						
					case GAMESCORE:
						if (LOG->GetNowUser()->getScore() == 0) {
							cout << "아직 기록이 없습니다." << endl;
						}
						else {
							cout << LOG->GetNowUser()->Getid() << " 플레이어의 현재 최고 기록은 " << LOG->GetNowUser()->getScore() << " 번 입니다." << endl;
						}
						continue;
						
					case LOGOUT:
						LOG->LogOut();
						break;

					case DEL:
						LOG->Delete();
						break;
					}
					break;
				}
			}
			break;

		case EXIT:
			flug = true;
			break;
		}
		if (flug) {
			break;
		}
	}
}