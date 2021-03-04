#include "User.h"

User::User(const char* _id, const char* _pass) {
	strcpy(id, _id);
	strcpy(pass, _pass);
	count = NODATA;
	money = GAMEMONEY;

	state = false;

	UserCharacterList = new ListNode<Character*>();
	SearchCharacter = nullptr;
}

User::~User() {
	if (UserCharacterList != nullptr) { delete UserCharacterList; }
}

// --------------------------------------------------------

void User::SetId(const char* _id) {
	strcpy(id, _id);
}

void User::SetPass(const char* _pass) {
	strcpy(pass, _pass);
}

void User::SetCount(int _count) {
	count = _count;
}

void User::SetMoney(int _money) {
	money = _money;
}

// --------------------------------------------------------

const char* User::GetId() {
	return id;
}

const char* User::GetPass() {
	return pass;
}

int User::GetCount() {
	return count;
}

int User::GetMoney() {
	return money;
}

// --------------------------------------------------------

bool User::Login() {
	state = true;
	return state;
}

bool User::LogOut() {
	state = false;
	return state;
}

void User::SearchEnd() {
	SearchCharacter = nullptr;
}

// --------------------------------------------------------

bool User::NewCharacter(Character* temp) {
	UserCharacterList->Insert(temp);
	count++;
	return true;
}

bool User::DelCharacter(Character* temp) {
	if (!UserCharacterList->Remove(temp)) {
		return false;
	}
	count--;
	return true;
}

bool User::CharacterCountCheak() {
	if (count < MAXCOUNT) {
		return true;
	}
	return false;
}

bool User::GetCharacter(Character** temp) {
	if (SearchCharacter == nullptr) {
		SearchCharacter = UserCharacterList->GetHead();
	}
	SearchCharacter = SearchCharacter->GetNext();

	if (SearchCharacter != nullptr) {
		*temp = SearchCharacter->GetData();
		return true;
	}
	return false;
}
