#pragma once

#include "Elf.h"
#include "Oak.h"
#include "Human.h"

#include "Portion.h"
#include "Armor.h"
#include "Wenpon.h"

#include "ListNode.h"

class CUser {
private:
	char id[CHARSIZE];		// ID
	char pass[CHARSIZE];	// PASSWORD
	int money;				// ������ ������ ��
	int count;				// ������ ������ ĳ���� ��
	bool state;				// ������ �α��� ��Ȳ

	ListNode<Character*>* UserCharList;	// ������ ������ ĳ���� ����Ʈ
	Node<Character*>* UserChar;			// ĳ���� ����Ʈ�� ��ȸ�ϱ� ���� ���

	ListNode<Item*>* UserItemList;		// ������ ������ ������ ����Ʈ
	Node<Item*>* UserItem;				// ������ ����Ʈ�� ��ȸ�ϱ� ���� ���
public:
	CUser(const char*, const char*);	// ������ (���̵�,���)
	~CUser();							// �Ҹ���

	// ����Ʈ ��ü ���� �Լ�
	void Init();

	// ���� ������
	void Setid(const char*);		
	void Setpass(const char*);
	void Setmoney(int);
	void Setcount(int);

	// �б� ������
	const char* Getid();
	const char* Getpass();
	int Getmoney();
	int Getcount();

	// �α��� üũ
	void Login() { state = true; }
	void Logout() { state = false; }

	// ������ ����,�Ǹ� �� ĳ���� ����,���� �� ���� üũ
	bool CheakMoney(int);		// ������ �� + �����ؾ� �� ���� 0 ���� ũ�� true
	bool CheakCharCount();		// ĳ���� �������� ������ ����Ʈ�� ���ϸ� true

	// ĳ���� �б�,���� ������ ��� �Լ�
	void SetChar(Node<Character*>*); // ĳ���� ��� ����
	Node<Character*>* Gethead();	// ĳ���� ����Ʈ�� ��� ��� �б�
	Node<Character*>* GetChar();	// ĳ���� ��� �б�

	void New(Character*);			// ����
	void Del(Node<Character*>*);	// ����

	// ������ �б�,���� ������ ��� �Լ�
	void SetItem(Node<Item*>*); // ������ ��� ����
	Node<Item*>* GetItemhead(); // ������ ����Ʈ�� ��� ��� �б�
	Node<Item*>* GetItem();	    // ������ ��� �б�

	void Buy(Item*);			// ������ ù ���� �� ����Ʈ ���� ����
	void Sell(Node<Item*>*);	// ������ �Ǹ� �� ������ �������� ���� �� ���� ����
};