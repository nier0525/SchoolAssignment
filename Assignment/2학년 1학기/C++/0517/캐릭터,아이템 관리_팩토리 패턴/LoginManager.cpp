#include "LoginManager.h"

LoginManager* LoginManager::pthis = nullptr;

LoginManager* LoginManager::GetInstance() {
	if (!pthis) {
		pthis = new LoginManager();
	}
	return pthis;
}

void LoginManager::Destory() {
	if (pthis) {
		delete pthis;
		pthis = nullptr;
	}
}

LoginManager::LoginManager() {
	UserList = nullptr;
	SearchUser = nullptr;
	NowUser = nullptr;
}

LoginManager::~LoginManager() {
	delete UserList;
}

void LoginManager::Init() {
	UserList = new ListNode<User*>();
}

User* LoginManager::GetNowUser() {
	return NowUser;
}

void LoginManager::New() {
	char id[CHARSIZE];
	char pass[CHARSIZE];

	while (1) {
		cout << "ID : ";
		cin >> id;

		SearchUser = UserList->GetHead()->GetNext();
		bool flug = false;

		while (SearchUser) {
			if (!strcmp(SearchUser->GetData()->GetId(), id)) {
				flug = true;
				break;
			}
			SearchUser = SearchUser->GetNext();
		}

		if (flug) {
			cout << "이미 존재하는 ID 입니다. 다시 입력하세요." << endl;
			continue;
		}

		break;
	}

	cout << "PASSWORD : ";
	cin >> pass;

	UserList->Insert(new User(id, pass));
}

bool LoginManager::Login() {
	char id[CHARSIZE];
	char pass[CHARSIZE];

	cout << "ID : ";
	cin >> id;

	SearchUser = UserList->GetHead()->GetNext();
	bool flug = false;

	while (SearchUser) {
		if (!strcmp(SearchUser->GetData()->GetId(), id)) {
			flug = true;
			break;
		}
		SearchUser = SearchUser->GetNext();
	}

	if (!flug) {
		cout << "없는 계정입니다." << endl;
		return false;
	}

	cout << "PASSWORD : ";
	cin >> pass;

	if (!strcmp(SearchUser->GetData()->GetPass(), pass)) {
		NowUser = SearchUser->GetData();
		NowUser->Login();
		cout << NowUser->GetId() << " 님이 로그인 하셨습니다." << endl;
		return true;
	}
	cout << "PASSWORD 가 틀렸습니다." << endl;
	return false;
}

void LoginManager::LogOut() {
	cout << NowUser->GetId() << " 님이 로그아웃 하셨습니다." << endl;
	NowUser->LogOut();
	NowUser = nullptr;
}

void LoginManager::Delete() {
	cout << NowUser->GetId() << " 님의 계정이 정상적으로 삭제되었습니다." << endl;
	UserList->Remove(NowUser);
	NowUser = nullptr;
}