#pragma once

#include "User.h"

// �α��� �Ŵ��� Ŭ����
// ���� ����,����,�α���,�α׾ƿ� ���� �����
// �̱��� ����

class LoginManager {
private:
	// ���� ����Ʈ
	ListNode<User*>* UserList;
	// ���� ����Ʈ ��ȸ ���
	Node<User*>* SearchUser;
	// ���� �������� ����
	User* NowUser;

	static LoginManager* pthis;
	LoginManager();
	~LoginManager();
public:
	// �̱��� �޼���
	static LoginManager* GetInstance();
	static void Destory();

	// ���� ���� ���� �б� ������
	User* GetNowUser();
	void Init(); // �ʱ�ȭ �޼���

	// ����,�α���,�α׾ƿ�,���� �޼���
	bool New(const char* id, const char* pass);
	bool Login(const char* id, const char* pass);
	void LogOut();
	void Delete();

	// ���� ����Ʈ ��ȸ �޼���
	bool GetUser(User**);
	// ��ȸ ��� �ʱ�ȭ
	void SearchEnd();

	void LoadUserList(User* user) {
		SearchUser = UserList->GetHead()->GetNext();
		// ��ȸ ��忡 ù ��带 ����

		while (SearchUser) { // ��ȸ ����
			if (!strcmp(SearchUser->GetData()->GetId(), user->GetId())) { // ���� ���� ����Ʈ �߿� �ߺ��� ���̵� �ִ� �� �˻�
				return; // ������ false
			}
			SearchUser = SearchUser->GetNext(); // ���� ���� �̵�
		}
		UserList->Insert(user);
	}
};