#pragma once

#include "User.h"
#include "ListNode.h"

class CLoginManager {
private:
	ListNode<CUser*>* ListUser;	// ���� ��ü�� ������ ����Ʈ
	Node<CUser*>* NowUser;		// �������� ������ ��Ī�� ���
public:
	CLoginManager();
	~CLoginManager();

	void Init();	// �ʱ�ȭ �۾�

	void New();		// ���� ��ü ����
	bool Login();	// �α��� üũ
	void Logout();	// �α׾ƿ� üũ
	void Del();		// ���� ��ü ����

	Node<CUser*>* GetNowUser() { return NowUser; }
	// ���� ������ ������ ������ ���� ��带 �ܺο� ���� �� �ְ� �ϴ� �б� ������
};