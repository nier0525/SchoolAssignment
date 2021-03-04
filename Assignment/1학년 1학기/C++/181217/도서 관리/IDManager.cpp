#include "IDMANAGER.h"

cIDManager::cIDManager() {
	memset(mIDlat, 0, sizeof(mIDlat));
	mCount = 0;
}

cIDManager::~cIDManager() {
	for (int i = 0; i < mCount; i++) {
		if (mIDlat[i] != nullptr) {
			delete mIDlat[i];
			mIDlat[i] = nullptr;
		}
	}
}

cIDMenu*	cIDManager::GetNowUser() {
	return mNowUser;
}

void cIDManager::New() {
	char _id[ID];
	char _pass[PASSWORD];

	while (1) {

		cout << "ID : ";
		cin >> _id;

		bool flug = false;

		for (int i = 0; i < mCount; i++) {
			if (strcmp(mIDlat[i]->GetId(), _id) == 0) {
				flug = true;
				break;
			}
		}
		if (flug) {
			cout << "입력한 ID 가 이미 존재합니다. 다시 입력하세요." << endl;
			continue;
		}
		break;
	}

	cout << "PASSWORD : ";
	cin >> _pass;

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
	cIDMenu*	_user = nullptr;

	while (1) {
		cout << "ID : ";
		cin >> _id;
		for (int i = 0; i < mCount; i++) {
			if (!strcmp(mIDlat[i]->GetId(), _id)) {
				_user = mIDlat[i];
				break;
			}
		}
		if (!_user) {
			cout << "없는 계정입니다." << endl;
			return false;
		}
		break;
	}
	while (1) {
		cout << "PASSWORD : ";
		cin >> _pass;
		if (!strcmp(_user->GetPass(), _pass)) {
			mNowUser = _user;
			mNowUser->Login();
			break;
		}
		else {
			cout << "PASSWORD 가 틀렸습니다." << endl;
			return false;
		}
		break;
	}
	return true;
}

void cIDManager::Rogout() {
	mNowUser->Logout();
}

void cIDManager::DeleteRog() {
	for (int i = 0; i < mCount; i++) {
		if (mIDlat[i] == mNowUser) {
			cout << mNowUser->GetId() << " 계정을 삭제합니다." << endl;
			delete mIDlat[i];
			for (int j = i; j < mCount - 1; j++) {
				mIDlat[j] = mIDlat[j + 1];
			}
			mIDlat[mCount - 1] = nullptr;
			break;
		}
	}
	mCount--;
}