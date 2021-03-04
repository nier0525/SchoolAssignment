#include "User.h"

// ������ �ʱ�ȭ �۾�
User::User(const char* _id, const char* _pass) {
	strcpy(id, _id);
	strcpy(pass, _pass);
	count = NODATA;

	state = false;

	UserCharacterList = new ListNode<Character*>();	// ������ ����Ʈ ���� �Ҵ�
	SearchCharacter = nullptr;
}

// �����Ҵ� �� ������ ����Ʈ �޸� ����
User::~User() {
	if (UserCharacterList != nullptr) { delete UserCharacterList; }
}

// -------------------------------------------------------- ���� ������

void User::SetId(const char* _id) {
	strcpy(id, _id);
}

void User::SetPass(const char* _pass) {
	strcpy(pass, _pass);
}

void User::SetCount(int _count) {
	count = _count;
}

// -------------------------------------------------------- �б� ������

const char* User::GetId() {
	return id;
}

const char* User::GetPass() {
	return pass;
}

int User::GetCount() {
	return count;
}

// --------------------------------------------------------

// �α���, �α׾ƿ��� ���� state �� ���� �� ��ȯ
bool User::Login() {
	state = true;
	return state;
}

bool User::LogOut() {
	state = false;
	return state;
}

// ��ȸ ��� �ʱ�ȭ
void User::SearchEnd() {
	SearchCharacter = nullptr;
}

 // --------------------------------------------------------

// ĳ���� ���� �޼���
bool User::NewCharacter(Character* temp) {
	UserCharacterList->Insert(temp);
	count++;
	return true;
}

// ĳ���� ���� �޼���
bool User::DelCharacter(Character* temp) {
	if (!UserCharacterList->Remove(temp)) {
		return false;
	}
	count--;
	return true;
}

// ĳ���� �ִ� ������ Ȯ�� �޼���
bool User::CharacterCountCheak() {
	if (count < MAXCOUNT) {
		return true;
	}
	return false;
}

// ĳ���� ����Ʈ ��ȸ
bool User::GetCharacter(Character** temp) {
	if (SearchCharacter == nullptr) {	// ��ȸ ��尡 nullptr ���
		SearchCharacter = UserCharacterList->GetHead();	// ��ȸ ��� ��ġ�� ù ���� �̵�
	}
	SearchCharacter = SearchCharacter->GetNext();	// ��ȸ ����

	if (SearchCharacter != nullptr) {	// ��ȸ ��尡 ��带 ����������
		*temp = SearchCharacter->GetData();	// �Ű������� ������ ���� �ѱ�
		return true;
	}
	return false;
}
