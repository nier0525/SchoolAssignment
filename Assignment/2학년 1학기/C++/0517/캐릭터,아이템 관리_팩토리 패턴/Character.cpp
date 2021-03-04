#include "Character.h"

Character::Character(const char* _name) {
	strcpy(Name, _name);
	BroodCode = NODATA;
	JobCode = NODATA;

	Level = 1;
	Hp = 500;
	Mp = 200;
	Att = 20;
	Def = 10;

	money = GAMEMONEY;
	Inven = new ListNode<Item*>();
	SearchItem = nullptr;
}

Character::~Character() {
	if (Inven != nullptr) {
		delete Inven;
		Inven = nullptr;
	}
}

//------------------------------------------------------

void Character::SetBroodCode(int _code) {
	BroodCode = _code;
}

void Character::SetJobCode(int _code) {
	JobCode = _code;
}

void Character::SetNickName(const char* _name) {
	strcpy(Name, _name);
}

void Character::SetHp(int _hp) {
	Hp = _hp;
}

void Character::SetMp(int _mp) {
	Mp = _mp;
}

void Character::SetAtt(int _att) {
	Att = _att;
}

void Character::SetDef(int _def) {
	Def = _def;
}

void Character::LevelUp() {
	Level++;
}

void Character::SetMoney(int _money) {
	money = _money;
}

//----------------------------------------------------

const char* Character::GetNickName() {
	return Name;
}

int Character::GetLevel() {
	return Level;
}

int Character::GetBroodCode() {
	return BroodCode;
}

int Character::GetJobCode() {
	return JobCode;
}

int Character::GetHp() {
	return Hp;
}

int Character::GetMp() {
	return Mp;
}

int Character::GetAtt() {
	return Att;
}

int Character::GetDef() {
	return Def;
}

int Character::GetMoney() {
	return money;
}

//----------------------------------------------------

Item* Character::Sell(const char* _code) {
	SearchItem = Inven->GetHead()->GetNext();
	while (SearchItem) {
		if (!strcmp(SearchItem->GetData()->GetItemCode(), _code)) {
			Item* ptr = SearchItem->GetData();
			this->InCreseGameMoney(ptr->GetPrice());
			Inven->Remove(SearchItem->GetData());
			this->SearchEnd();
			return ptr;
		}
		SearchItem = SearchItem->GetNext();
	}
	return nullptr;
}

bool Character::Buy(Item* _item) {
	if (_item != nullptr) {
		if (this->DeCreseGameMoney(_item->GetPrice())) {
			Inven->Insert(_item);
			return true;
		}
	}
	else {
		return false;
	}
}

bool Character::InCreseGameMoney(int _money) {
	money += _money;
	return true;
}

bool Character::DeCreseGameMoney(int _money) {
	if (money - _money < NODATA) {
		return false;
	}
	else {
		money -= _money;
		return true;
	}
}

//----------------------------------------------------

void Character::SearchEnd() {
	SearchItem = nullptr;
}

bool Character::GetItem(Item** temp) {
	if (SearchItem == nullptr) {
		SearchItem = Inven->GetHead();
	}

	SearchItem = SearchItem->GetNext();

	if (SearchItem != nullptr) {
		*temp = SearchItem->GetData();
		return true;
	}
	return false;
}