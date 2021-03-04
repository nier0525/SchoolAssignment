#include "LoginManager.h"

LoginManager::LoginManager() {
	ListUser = nullptr;
	NowUser = nullptr;
}

LoginManager::~LoginManager() {
	//Node<CUser*>* node = ListUser->Gethead()->Getlink();
	//while (node) {
	//	if (node->GetData() != nullptr) {
	//		delete node->GetData();
	//	}
	//	node = node->Getlink();
	//}
	delete ListUser, NowUser;
}

void LoginManager::init() {
	ListUser = new ListNode<CUser*>();
	ListUser->init();
}

Node<CUser*>* LoginManager::UserCheak(CUser* _user) {
	NowUser = ListUser->Gethead();
	while (NowUser != nullptr) {
		if (!strcmp(NowUser->GetData()->Getid(), _user->Getid())) {
			return NowUser;
		}
		else {
			NowUser = NowUser->Getlink();
		}
	}
	return NowUser;
}

void LoginManager::New() {

	char id[CHARSIZE];
	char pass[CHARSIZE];

	CUser* User = new CUser();

	cout << "ID : ";
	cin >> id;
	User->Setid(id);
	
	if (UserCheak(User) != nullptr) {
		cout << "같은 ID 가 존재합니다." << endl;
		delete User;
		return;
	}

	cout << "PASSWORD : ";
	cin >> pass;
	User->SetPass(pass);

	ListUser->Insert(User);
}

bool LoginManager::Login() {

	char id[CHARSIZE];
	char pass[CHARSIZE];

	CUser* User = new CUser();

	cout << "ID : ";
	cin >> id;
	User->Setid(id);

	if (UserCheak(User) != nullptr) {
		NowUser = UserCheak(User);
	}
	else {
		cout << "없는 ID 입니다." << endl;
		delete User;
		return false;
	}

	cout << "PASSWORD : ";
	cin >> pass;

	if (!strcmp(NowUser->GetData()->GetPass(),pass)) {
		NowUser->GetData()->Login();
		return true;
	}
	else {
		cout << "PASSWORD 가 틀렸습니다." << endl;
		NowUser = nullptr;
		delete User;
		return false;
	}
}

void LoginManager::Logout() {
	NowUser->GetData()->LogOut();
	NowUser = nullptr;
}

void LoginManager::Del() {
	ListUser->Delete(NowUser);
	NowUser = nullptr;
}