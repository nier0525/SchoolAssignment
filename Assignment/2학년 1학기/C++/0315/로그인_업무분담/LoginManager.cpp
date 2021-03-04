#include "LoginManager.h"

CLogin::CLogin() {
	memset(ListUser, 0, sizeof(ListUser));
	mCount = 0;
	mNowUser = nullptr;
}

CLogin::~CLogin() {
	for (int i = 0; i < mCount; i++) {
		if (ListUser[i] != nullptr) {
			delete ListUser[i];
			ListUser[i] = nullptr;
		}
	}
}

void CLogin::New() {
	char _id[NAMESIZE];
	char _pass[PASSSIZE];
	bool flug = false;

	while (1) {
		cout << "ID : ";
		cin >> _id;
		for (int i = 0; i < mCount; i++) {
			if (!strcmp(ListUser[i]->Getid(), _id)) {
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

	if (mCount >= USERSIZE) {
		cout << "�ʹ� ���� ������ �����߽��ϴ�. ������ ������ �����ϰ� �ٽ� �õ��ϼ���." << endl;
		return;
	}

	ListUser[mCount++] = new CUser(_id, _pass);
}

bool CLogin::Login() {
	char _id[NAMESIZE];
	char _pass[PASSSIZE];

	cout << "ID : ";
	cin >> _id;
	for (int i = 0; i < mCount; i++) {
		if (!strcmp(ListUser[i]->Getid(), _id)) {
			mNowUser = ListUser[i];
		}
	}
	if (mNowUser == nullptr) {
		cout << "ID �� ���ų�, �߸� �Է��ϼ̽��ϴ�." << endl;
		return false;
	}

	cout << "PASSWORD : ";
	cin >> _pass;

	if (!strcmp(mNowUser->GetPass(), _pass)) {
		mNowUser->Login();
		return true;
	}
	else {
		cout << "PASSWORD �� ���ų�, �߸� �Է��ϼ̽��ϴ�." << endl;
		return false;
	}
}

CUser* CLogin::GetNowUser() {
	return mNowUser;
}

void CLogin::LogOut() {
	mNowUser->LogOut();
	mNowUser = nullptr;
}

void CLogin::Delete() {
	for (int i = 0; i < mCount; i++) {
		if (ListUser[i] != nullptr && ListUser[i] == mNowUser) {
			delete ListUser[i];
			for (int j = i; j < mCount - 1; j++) {
				ListUser[j] = ListUser[j + 1];
			}
			ListUser[mCount - 1] = nullptr;
		}
	}
	mCount--;
	mNowUser = nullptr;
}