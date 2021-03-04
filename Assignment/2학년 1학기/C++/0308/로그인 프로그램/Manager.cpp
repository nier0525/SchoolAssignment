#include "Manager.h"

Manager::Manager() {
	memset(lstuser, 0, sizeof(lstuser));
	now = nullptr;
	conut = 0;
}

Manager::~Manager() {
	for (int i = 0; i < conut; i++) {
		if (lstuser[i] != nullptr) {
			delete lstuser[i];
			lstuser[i] = nullptr;
		}
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
			cout << "���� ID �� �̹� �ֽ��ϴ�." << endl;
			flug = false;
			continue;
		}
		break;
	}

	cout << "PASSWORD : ";
	cin >> _pass;

	if (conut >= USERSIZE) {
		cout << "�ʹ� ���� ������ �����߽��ϴ�. ������ ������ �����ϰ� �ٽ� �õ��ϼ���." << endl;
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
		cout << "ID �� ���ų�, �߸� �Է��ϼ̽��ϴ�." << endl;
		return false;
	}

	cout << "PASSWORD : ";
	cin >> _pass;

	if (!strcmp(now->GetPass(), _pass)) {
		now->Login();
		return true;
	}
	else {
		cout << "PASSWORD �� ���ų�, �߸� �Է��ϼ̽��ϴ�." << endl;
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

		cout << "<MainMenu>" << endl << "1. ȸ������" << endl << "2. �α���" << endl << "3. ����" << endl << "���� : ";
		cin >> sel;

		switch (sel) {
		case JOIN:
			NewLog();
			break;

		case LOGIN:
			if (Login()) {
				cout << "<LoginMenu>" << endl << "1. �α׾ƿ�" << endl << "2. ȸ��Ż��" << endl << "���� : ";
				cin >> sel;
				switch (sel) {

				case LOGOUT:
					Logout();
					break;

				case DEL:
					Del();
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