#include "Character.h"

Character::Character(const char* _name) {
	strcpy(Name, _name);
	BroodCode = NODATA;
	Level = 1;
	HP = 500;
	MP = 50;
	Att = 20;
	Def = 10;

	CharItemList = new ListNode<Item*>();
	SearchItem = nullptr;
}

Character::~Character() {
	if (CharItemList != nullptr) {
		delete CharItemList;
	}
}

//---------------------------------------------------

void Character::SetCharName(const char* _name) {
	strcpy(Name, _name);
}

void Character::SetBroodCode(int _code) {
	BroodCode = _code;
}

void Character::LevelUp() {
	Level++;
}

void Character::SetHP(int _point) {
	HP = _point;
}

void Character::SetMP(int _point) {
	MP = _point;
}

void Character::SetAtt(int _point) {
	Att = _point;
}

void Character::SetDef(int _point) {
	Def = _point;
}

//----------------------------------------------------

const char* Character::GetCharName() {
	return Name;
}

int Character::GetBroodCode() {
	return BroodCode;
}

int Character::GetLevel() {
	return Level;
}

int Character::GetHP() {
	return HP;
}

int Character::GetMP() {
	return MP;
}

int Character::GetAtt() {
	return Att;
}

int Character::GetDef() {
	return Def;
}

//----------------------------------------------------

bool Character::OnItem(Item* _item) {
	CharItemList->Insert(_item);
	_item->OnItem(this);
	return true;
}

bool Character::OffItem(Item* _item) {
	_item->OffItem(this);
	return (CharItemList->Delete(_item));
}

bool Character::GetItem(Item** index) {
	if (SearchItem == nullptr) {
		SearchItem = CharItemList->GetHead();
	}

	SearchItem = SearchItem->GetLink();

	if (SearchItem != nullptr) {
		*index = SearchItem->GetData();
		return true;
	}
	return false;
}

void Character::SearchItemEnd() {
	SearchItem = nullptr;
}