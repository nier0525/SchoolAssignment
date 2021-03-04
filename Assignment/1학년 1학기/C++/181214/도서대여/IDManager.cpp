#include "IDMANAGER.h"

cIDManager::cIDManager() {
	for (int i = 0; i < MAXID; i++) {
		mIDlat[i] = nullptr;
	}
	mCount = 0;
}

cIDManager::~cIDManager() {
	for (int i = 0; i < mCount; i++) {
		delete mIDlat[i];
	}
}

void cIDManager::New() {
	char _id[ID];
	char _pass[PASSWORD];

	cout << "ID : ";
	cin >> _id;
	cout << "PASSWORD : ";
	cin >> _pass;

	for (int i = 0; i < mCount; i++) {
		if (!strcmp(mIDlat[i]->GetId(), _id)) {
			cout << "�Է��� ID �� �̹� �����մϴ�." << endl;
			break;
		}
	}

	cIDMenu* ptr = new cIDMenu(_id, _pass);
	if (_id[ID] < ID && _pass[PASSWORD] < PASSWORD) {
		mIDlat[mCount++] = ptr;
	}
	else {
		cout << "error! ID Ȥ�� PASSWORD �� ���̸� ���̼���." << endl;
		delete ptr;
	}
}

bool cIDManager::Rogin() {
	
	char _id[ID];
	char _pass[PASSWORD];

	cout << "ID : ";
	cin >> _id;
	cout << "PASSWORD : ";
	cin >> _pass;

	for (int i = 0; i < mCount; i++) {
		if (!strcmp(mIDlat[i]->GetId(), _id) && !strcmp(mIDlat[i]->GetPass(), _pass)) {
			cout << mIDlat[i]->GetId() << " ���� �α��� �ϼ̽��ϴ�." << endl;
			return true;
		}
	}
	cout << "���� �����Դϴ�." << endl;
	return false;
}

void cIDManager::DeleteRog() {

	cout << "�����Ͻ� ID �� PASSWORD �� �Է��ϼ���" << endl;

	char _id[ID];
	char _pass[PASSWORD];

	cout << "ID : ";
	cin >> _id;
	cout << "PASSWORD : ";
	cin >> _pass;

	for (int i = 0; i < mCount; i++) {
		if (!strcmp(mIDlat[i]->GetId(), _id) && !strcmp(mIDlat[i]->GetPass(), _pass)) {
			delete mIDlat[i];
		}
	}
}