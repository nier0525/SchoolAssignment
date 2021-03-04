#include "Manager.h"

Manager::Manager() {
	memset(lstuser, 0, sizeof(lstuser));
	now = nullptr;
	conut = 0;
	GM = nullptr;
}

Manager::~Manager() {
	for (int i = 0; i < conut; i++) {
		if (lstuser[i] != nullptr) {
			delete lstuser[i];
			lstuser[i] = nullptr;
		}
	}
	if (now != nullptr) {
		now = nullptr;
	}
	if (GM != nullptr) {
		GM = nullptr;
	}
}

void Manager::NewLog() {
	char _id[NAMESIZE];
	char _pass[PASSSIZE];
	bool flug = false;

	while (1) {
		cout << "ID : ";
		cin >> _id;
		for (int i = 0; i < conut; i++) {
			if (!strcmp(lstuser[i]->Getid(), _id)) {
				flug = true;
				break;
			}
		}
		if (flug) {
			cout << "같은 ID 가 이미 있습니다." << endl;
			flug = false;
			continue;
		}
		break;
	}

	cout << "PASSWORD : ";
	cin >> _pass;

	if (conut >= USERSIZE) {
		cout << "너무 많은 계정을 생성했습니다. 기존의 계정을 삭제하고 다시 시도하세요." << endl;
		return;
	}

	lstuser[conut++] = new CUser(_id, _pass);
}

bool Manager::Login() {
	char _id[NAMESIZE];
	char _pass[PASSSIZE];

	now = nullptr;

	cout << "ID : ";
	cin >> _id;
	for (int i = 0; i < conut; i++) {
		if (!strcmp(lstuser[i]->Getid(), _id)) {
			now = lstuser[i];
		}
	}
	if (now == nullptr) {
		cout << "ID 가 없거나, 잘못 입력하셨습니다." << endl;
		return false;
	}

	cout << "PASSWORD : ";
	cin >> _pass;

	if (!strcmp(now->GetPass(), _pass)) {
		now->Login();
		return true;
	}
	else {
		cout << "PASSWORD 가 없거나, 잘못 입력하셨습니다." << endl;
		return false;
	}
}

void Manager::Logout() {
	now->LogOut();
}

void Manager::Del() {
	for (int i = 0; i < conut; i++) {
		if (lstuser[i] != nullptr && lstuser[i] == now) {
			delete lstuser[i];
			for (int j = i; j < conut - 1; j++) {
				lstuser[j] = lstuser[j + 1];
			}
			lstuser[conut - 1] = nullptr;
		}
	}
	conut--;
}

void Manager::Run() {

	while (1) {
		int sel;
		bool flug = false;

		cout << "<MainMenu>" << endl << "1. 회원가입" << endl << "2. 로그인" << endl << "3. 종료" << endl << "선택 : ";
		cin >> sel;

		switch (sel) {
		case JOIN:
			NewLog();
			break;

		case LOGIN:
			if (Login()) {
				while (1) {

					cout << "<LoginMenu>" << endl << "1. 게임시작 " << endl << "2. 기록확인 " << endl << "3. 로그아웃" << endl << "4. 회원탈퇴" << endl << "선택 : ";
					cin >> sel;
					switch (sel) {

					case GAMESTART:
						GM = new GManager();
						if (GM->Run()) {
							int index;
							index = GM->GetScore();
							now->SetScore(index);
							cout << "Your Win !" << endl;
							cout << now->Getid() << " 플레이어의 최고 기록은 " << now->getScore() << " 번 입니다." << endl;
						}
						GM = nullptr;
						continue;
						
					case GAMESCORE:
						if (now->getScore() == 0) {
							cout << "아직 기록이 없습니다." << endl;
						}
						else {
							cout << now->Getid() << " 플레이어의 현재 최고 기록은 " << now->getScore() << " 번 입니다." << endl;
						}
						continue;

					case LOGOUT:
						Logout();
						break;

					case DEL:
						Del();
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