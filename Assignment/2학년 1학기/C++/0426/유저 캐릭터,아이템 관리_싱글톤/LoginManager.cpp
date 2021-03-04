#include "LoginManager.h"

LoginManager* LoginManager::Pthis = nullptr;

LoginManager* LoginManager::GetInstance() {
	if (!Pthis) {
		Pthis = new LoginManager();
	}
	return Pthis;
}

void LoginManager::Destory() {
	if (Pthis) {
		delete Pthis;
		Pthis = nullptr;
	}
}

LoginManager::LoginManager() {
	ListUser = nullptr;
	SearchUser = nullptr;
	NowUser = nullptr;
}

LoginManager::~LoginManager() {
	delete ListUser;
}

void LoginManager::Init() {
	ListUser = new ListNode<User*>();
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
		
		SearchUser = ListUser->GetHead()->GetLink();
		bool flug = false;

		while (SearchUser) {
			if (!strcmp(SearchUser->GetData()->GetId(), id)) {
				flug = true;
				break;
			}
			SearchUser = SearchUser->GetLink();
		}

		if (flug) {
			cout << "�̹� �����ϴ� ID �Դϴ�. �ٽ� �Է��ϼ���." << endl;
			continue;
		}

		break;
	}

	cout << "PASSWORD : ";
	cin >> pass;

	ListUser->Insert(new User(id, pass));
}

bool LoginManager::Login() {
	char id[CHARSIZE];
	char pass[CHARSIZE];

	cout << "ID : ";
	cin >> id;

	SearchUser = ListUser->GetHead()->GetLink();
	bool flug = false;

	while (SearchUser) {
		if (!strcmp(SearchUser->GetData()->GetId(), id)) {
			flug = true;
			break;
		}
		SearchUser = SearchUser->GetLink();
	}

	if (!flug) {
		cout << "���� ID �Դϴ�." << endl;
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

void LoginManager::Logout() {
	cout << NowUser->GetId() << " ���� �α׾ƿ� �ϼ̽��ϴ�." << endl;
	NowUser->Logout();
	NowUser = nullptr;
}

void LoginManager::Delete() {
	cout << NowUser->GetId() << " ���� ȸ��Ż�� �Ǿ����ϴ�." << endl;
	ListUser->Remove(NowUser);
	NowUser = nullptr;
}