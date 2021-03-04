#include "User.h"
#include "CharacterManager.h"

User::User(const char* _id, const char* _pass) {
	strcpy(id, _id);
	strcpy(pass, _pass);
	state = false;

	UserCharList = new ListNode<Character*>();
	SearchChar = nullptr;
	count = NODATA;

	UserItemList = new ListNode<Item*>();
	SearchItem = nullptr;
	money = GAMEMONEY;
}

User::~User() {
	if (UserCharList != nullptr) {
		delete UserCharList;
	}
	if (UserItemList != nullptr) {
		delete UserItemList;
	}
}

//--------------------------------------------

void User::SetId(const char* _id) {
	strcpy(id, _id);
}

void User::SetPass(const char* _pass) {
	strcpy(pass, _pass);
}

bool User::NewCharacter(Character* index) {
	UserCharList->Insert(index);
	count++;
	return true;
}

bool User::DelCharacter(Character* index) {
	Item* item = nullptr;
	while (index->GetItem(&item)) {
		UserItemList->Insert(item);
	}
	index->SearchItemEnd();

	if (!UserCharList->Remove(index)) {
		return false;
	}
	count--;
	return true;
}

bool User::CharCountCheck() {
	if (count < MAXCOUNT) {
		return true;
	}
	else {
		return false;
	}
}

Item* User::Sell(const char* _code) {
	SearchItem = UserItemList->GetHead()->GetLink();
	while (SearchItem) {
		if (!strcmp(SearchItem->GetData()->GetItemCode(), _code)) {
			Item* item = SearchItem->GetData();
			this->InCreseGameMoney(item->GetPrice());
			UserItemList->Remove(SearchItem->GetData());
			this->SearchItemEnd();
			return item;
		}
		SearchItem = SearchItem->GetLink();
	}
	return nullptr;
}

bool User::Buy(Item* _item) {
	if (this->DeCreseGameMoney(_item->GetPrice())) {
		UserItemList->Insert(_item);
		return true;
	}
	else {
		return false;
	}
}

bool User::InCreseGameMoney(int _money) {
	money += _money;
	return true;
}

bool User::DeCreseGameMoney(int _money) {
	if (money - _money < NODATA) {
		return false;
	}
	else {
		money -= _money;
		return true;
	}
}

//--------------------------------------------

const char* User::GetId() {
	return id;
}

const char* User::GetPass() {
	return pass;
}

int User::GetGameMoney() {
	return money;
}

int User::GetCharCount() {
	return count;
}

//--------------------------------------------

void User::Login() {
	state = true;
}

void User::Logout() {
	state = false;
}

void User::SearchCharEnd() {
	SearchChar = nullptr;
}

void User::SearchItemEnd() {
	SearchItem = nullptr;
}

bool User::GetCharacter(Character** index) {
	if (SearchChar == nullptr) {
		SearchChar = UserCharList->GetHead();
	}
	
	SearchChar = SearchChar->GetLink();

	if (SearchChar != nullptr) {
		*index = SearchChar->GetData();
		return true;
	}
	return false;
}

bool User::GetItem(Item** index) {
	if (SearchItem == nullptr) {
		SearchItem = UserItemList->GetHead();
	}

	SearchItem = SearchItem->GetLink();

	if (SearchItem != nullptr) {
		*index = SearchItem->GetData();
		return true;
	}
	return false;
}