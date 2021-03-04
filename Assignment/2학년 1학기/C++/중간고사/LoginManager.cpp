#include "LoginManager.h"

// ������ �ʱ�ȭ �۾�
CLoginManager::CLoginManager() {
	ListUser = nullptr;
	NowUser = nullptr;
}

// �Ҹ��� ��ȯ �۾�
CLoginManager::~CLoginManager() {
	if (ListUser->GetHead() != nullptr) {
		delete ListUser;
	}
	if (NowUser != nullptr) {
		delete NowUser;
	}
}

// �ʱ�ȭ ����
void CLoginManager::Init() {
	ListUser = new ListNode<CUser*>(); // ���� ����Ʈ ����
}

// ���� ����
void CLoginManager::New() {
	char id[CHARSIZE];
	char pass[CHARSIZE];

	while (1) {
		cout << "ID : ";
		cin >> id;

		bool flug = false;
		NowUser = ListUser->GetHead();
		// ���� ����Ʈ�� ��� ��� ��ġ�� ���� ��带 ��ġ
		while (NowUser != nullptr) {	// ���� ��尡 nullptr �� ����������
			if (!strcmp(NowUser->GetData()->Getid(), id)) {
				// ����Ʈ ���� ��� �� �Է� ���� id ���� ���� ��尡 ���� ���
				flug = true;
				break; // while ����
			}
			NowUser = NowUser->GetNext(); // ���� ���� �̵�
		}

		if (flug) {	// ���� id�� ã�� ���
			cout << "�ߺ��� ID �� �ֽ��ϴ�. �ٽ� �Է��ϼ���." << endl;
			continue; // ó������ �ٽ� ����
		}
		break; // while ����
	}

	cout << "PASSWORD : ";
	cin >> pass;

	CUser* ptr = new CUser(id, pass);	// ���� ��ü ���� 
	ptr->Init();	// ������ ���� ��ü�� �ʱ�ȭ�� ���� �Լ�
	ListUser->Input(ptr);	// ���� ����Ʈ�� �߰�
}

bool CLoginManager::Login() {
	char id[CHARSIZE];
	char pass[CHARSIZE];

	cout << "ID : ";
	cin >> id;

	NowUser = ListUser->GetHead();
	// ���� ����Ʈ�� ��� ��� ��ġ�� ���� ��带 ��ġ
	while (NowUser != nullptr) {	// ���� ��尡 nullptr �� ����������
		if (!strcmp(NowUser->GetData()->Getid(), id)) {
			// ����Ʈ ���� ��� �� �Է� ���� id ���� ���� ��尡 ���� ���
			break; // while �� Ż��
			// Ż�������ν� NowUser �� ����Ʈ ���� id ���� ��ġ�ϴ� ����� �ּҿ� �ӹ���.
		}
		NowUser = NowUser->GetNext(); // ���� ���� �̵�
	}
	if (NowUser == nullptr) {	// ��尡 nullptr ���� �̵��� ��� (id �� ã�� ���� ���)
		cout << "���� �����Դϴ�." << endl;
		return false;
	}

	cout << "PASSWORD : ";
	cin >> pass;

	if (!strcmp(NowUser->GetData()->Getpass(), pass)) {
		// NowUser �� ����Ű�� ����� ���� ������ �� pass ���� �Է� ���� pass �� ��ġ�ϴ� ���
		NowUser->GetData()->Login(); // ���� �α��� ���·� üũ
		return true;
	}
	else { // pass �� ��ġ���� �ʴ� ���
		cout << "PASSWORD �� ��ġ���� �ʽ��ϴ�." << endl;
		return false;
	}
}

void CLoginManager::Logout() {
	NowUser->GetData()->Logout();	// ���� ������ ������ �α׾ƿ� ���·� üũ
	NowUser = nullptr; // ���� ��� �ʱ�ȭ
}

void CLoginManager::Del() {
	ListUser->Delete(NowUser); // ���� ������ ������ ��带 ����Ʈ���� ����
	NowUser = nullptr;		// ���� ��� �ʱ�ȭ
}