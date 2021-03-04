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
			cout << "�̹� �����ϴ� ID �Դϴ�. �ٽ� �Է��ϼ���." << endl;
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
		cout << "���� �����Դϴ�." << endl;
		return false;
	}

	cout << "PASSWORD : ";
	cin >> pass;

	if (!strcmp(SearchUser->GetData()->GetPass(), pass)) {
		NowUser = SearchUser->GetData();
		NowUser->Login();
		cout << NowUser->GetId() << " ���� �α��� �ϼ̽��ϴ�." << endl;
		return true;
	}
	cout << "PASSWORD �� Ʋ�Ƚ��ϴ�." << endl;
	return false;
}

void LoginManager::LogOut() {
	cout << NowUser->GetId() << " ���� �α׾ƿ� �ϼ̽��ϴ�." << endl;
	NowUser->LogOut();
	NowUser = nullptr;
}

void LoginManager::Delete() {
	cout << NowUser->GetId() << " ���� ������ ���������� �����Ǿ����ϴ�." << endl;
	UserList->Remove(NowUser);
	NowUser = nullptr;
}