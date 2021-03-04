#include "User.h"

CUser::CUser(const char* _id, const char* _pass) {
	strcpy(id, _id);
	strcpy(pass, _pass);
	money = GAMEMONEY;		// ������ ���� ������ ����Ʈ ������ �ʱ�ȭ
	count = NODATA;			// ������ ĳ���� ������ NODATA
}

CUser::~CUser() {
	delete UserCharList, UserItemList;		// ����Ʈ �޸� ��ȯ
	delete UserChar, UserItem;				// ��� �޸� ��ȯ
}

void CUser::Init() {
	UserCharList = new ListNode<Character*>();  // ĳ���� ����Ʈ ���� �Ҵ�
	UserChar = nullptr;							// ĳ���� ��� �ʱ�ȭ
	UserItemList = new ListNode<Item*>();		// ������ ����Ʈ ���� �Ҵ�
	UserItem = nullptr;							// ������ ��� �ʱ�ȭ
}

// ���� ������ �Լ�

void CUser::Setid(const char* _id) {
	strcpy(id, _id);
}

void CUser::Setpass(const char* _pass) {
	strcpy(pass, _pass);
}

void CUser::Setmoney(int _money) {
	money = _money;
}

void CUser::Setcount(int _count) {
	count = _count;
}

// �б� ������ �Լ�

const char* CUser::Getid() {
	return id;
}

const char* CUser::Getpass() {
	return pass;
}

int CUser::Getmoney() {
	return money;
}

int CUser::Getcount() {
	return count;
}

// ĳ���� ����,���� �� ������ ����,�ǸŽ� ���� üũ �Լ�

bool CUser::CheakMoney(int _money) {
	if (money - _money >= NODATA) {
		return true;
	}
	else {
		return false;
	}
}

bool CUser::CheakCharCount() {
	if (count < MAXCOUNT) {
		return true;
	}
	else {
		return false;
	}
}

// ĳ���Ϳ� ���� �Լ�
void CUser::SetChar(Node<Character*>* _char) {	// ĳ���� ��� ����
		UserChar = _char;
}

Node<Character*>* CUser::GetChar() {		// ĳ���� ��� �б�
	return UserChar;
}

Node<Character*>* CUser::Gethead() {		// ĳ���� ����Ʈ�� ��� ��� �б�
	return UserCharList->GetHead();
}

void CUser::New(Character* _char) {			// ĳ���� ����
	UserCharList->Input(_char);
	count++;
}

void CUser::Del(Node<Character*>* _char) {	// ĳ���� ����
	UserCharList->Delete(_char);
	count--;
}

// �����ۿ� ���� �Լ�

void CUser::SetItem(Node<Item*>* _item) {	// ������ ��� ����
	UserItem = _item;
}

Node<Item*>* CUser::GetItem() {				// ������ ��� �б�
	return UserItem;
}

Node<Item*>* CUser::GetItemhead() {			// ������ ����Ʈ�� ��� ��� �б�
	return UserItemList->GetHead();
}

void CUser::Buy(Item* _item) {				// ������ ù ���� �� ��� ���� �Լ�
	UserItemList->Input(_item);
}

void CUser::Sell(Node<Item*>* _item) {		// ������ �Ǹ� �� �������� ���� �� ��� ���� �Լ�
    UserItemList->Delete(_item);
}
