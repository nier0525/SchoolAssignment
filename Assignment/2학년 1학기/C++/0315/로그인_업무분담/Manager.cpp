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

		cout << "<MainMenu>" << endl << "1. ȸ������" << endl << "2. �α���" << endl << "3. ����" << endl << "���� : ";
		cin >> sel;

		switch (sel) {
		case JOIN:
			LOG->New();
			break;

		case LOGIN:
			if (LOG->Login()) {
				while (1) {

					cout << "<LoginMenu>" << endl << "1. ���ӽ��� " << endl << "2. ���Ȯ�� " << endl << "3. �α׾ƿ�" << endl << "4. ȸ��Ż��" << endl << "���� : ";
					cin >> sel;
					switch (sel) {

					case GAMESTART:
						GM->Gameinit(LOG->GetNowUser());
						GM->GameRun();
						GM->GameEnd();
						continue;
						
					case GAMESCORE:
						if (LOG->GetNowUser()->getScore() == 0) {
							cout << "���� ����� �����ϴ�." << endl;
						}
						else {
							cout << LOG->GetNowUser()->Getid() << " �÷��̾��� ���� �ְ� ����� " << LOG->GetNowUser()->getScore() << " �� �Դϴ�." << endl;
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