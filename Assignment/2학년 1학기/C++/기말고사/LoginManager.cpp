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

// ������ �ʱ�ȭ
LoginManager::LoginManager() {
	UserList = nullptr;
	SearchUser = nullptr;
	NowUser = nullptr;
}

// ���� ����Ʈ �޸� ����
LoginManager::~LoginManager() {
	delete UserList;
}

void LoginManager::Init() {
	UserList = new ListNode<User*>();	// ���� ����Ʈ ���� �Ҵ�
}

// ���� �������� ���� ��ü ��ȯ
User* LoginManager::GetNowUser() {
	return NowUser;
}

// ���� ����
bool LoginManager::New(const char* id, const char* pass) {
	SearchUser = UserList->GetHead()->GetNext();
	// ��ȸ ��忡 ù ��带 ����

	while (SearchUser) { // ��ȸ ����
		if (!strcmp(SearchUser->GetData()->GetId(), id)) { // ���� ���� ����Ʈ �߿� �ߺ��� ���̵� �ִ� �� �˻�
			return false; // ������ false
		}
		SearchUser = SearchUser->GetNext(); // ���� ���� �̵�
	}
	
	UserList->Insert(new User(id, pass)); // ���� ����Ʈ�� �߰�
	return true;
}

// �α���
bool LoginManager::Login(const char* id, const char* pass) {
	SearchUser = UserList->GetHead()->GetNext();
	// ��ȸ ��忡 ù ��带 ����
	bool flug = false;

	while (SearchUser) { // ��ȸ ����
		if (!strcmp(SearchUser->GetData()->GetId(), id)) { // ���� ���� ����Ʈ�� �Է� ���� ���̵� �ִٸ�
			flug = true; // �ο��� ��
			break; // ��ȸ �ߴ�
		}
		SearchUser = SearchUser->GetNext(); // ���� ����
	}

	// ���� ������ ã�� ������ ���
	if (!flug) {
		return false;
	}

	// �н�������� ��ġ�Ѵٸ�
	if (!strcmp(SearchUser->GetData()->GetPass(), pass)) {
		NowUser = SearchUser->GetData(); // ���� �������� ������ ��ü�� ����
		NowUser->Login(); // �α��� ����
		return true;
	}

	// �н����� ����ġ ��
	return false;
}

// �α׾ƿ�
void LoginManager::LogOut() {
	NowUser->LogOut(); // �α׾ƿ� ����
	NowUser = nullptr; // �ʱ�ȭ
}

// ����
void LoginManager::Delete() {
	UserList->Remove(NowUser); // ���� �������� ���� ������ �����Ѵ�.
	NowUser = nullptr; // �ʱ�ȭ
}

// ��ȸ ��� �ʱ�ȭ
void LoginManager::SearchEnd() {
	SearchUser = nullptr;
}

// ���� ����Ʈ ��ȸ
bool LoginManager::GetUser(User** temp) {
	if (SearchUser == nullptr) { // ��ȸ ��尡 nullptr ���
		SearchUser = UserList->GetHead(); // ��ȸ ����� ��ġ�� ù ����
	}

	SearchUser = SearchUser->GetNext(); // ��ȸ ����

	if (SearchUser != nullptr) {	// ��带 ������
		*temp = SearchUser->GetData(); // �Ű� �������� ����� �����͸� �Ѱ���
		return true;
	}
	return false;
}