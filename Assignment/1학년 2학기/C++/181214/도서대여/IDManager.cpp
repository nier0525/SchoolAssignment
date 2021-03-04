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
			cout << "입력한 ID 가 이미 존재합니다." << endl;
			break;
		}
	}

	cIDMenu* ptr = new cIDMenu(_id, _pass);
	if (_id[ID] < ID && _pass[PASSWORD] < PASSWORD) {
		mIDlat[mCount++] = ptr;
	}
	else {
		cout << "error! ID 혹은 PASSWORD 의 길이를 줄이세요." << endl;
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
			cout << mIDlat[i]->GetId() << " 님이 로그인 하셨습니다." << endl;
			return true;
		}
	}
	cout << "없는 계정입니다." << endl;
	return false;
}

void cIDManager::DeleteRog() {

	cout << "삭제하실 ID 와 PASSWORD 를 입력하세요" << endl;

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