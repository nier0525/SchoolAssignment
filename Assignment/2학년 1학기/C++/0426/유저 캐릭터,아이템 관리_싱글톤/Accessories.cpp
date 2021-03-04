#include "Accessories.h"
#include "Character.h"

int Accessories::Num = NODATA;

Accessories::Accessories(const char* _name, int _price, int _HP ,int _MP) : Item(_name, _price) {
	HP = _HP;
	MP = _MP;
	Item::Setkind(ACCESSORIES);
}

Accessories::~Accessories() {

}

int Accessories::GetFirstAbility() {
	return HP;
}

int Accessories::GetSecordAbility() {
	return MP;
}

bool Accessories::OnItem(Character* index) {
	index->SetHP(index->GetHP() + HP);
	index->SetMP(index->GetMP() + MP);
	return true;
}

bool Accessories::OffItem(Character* index) {
	index->SetHP(index->GetHP() - HP);
	index->SetMP(index->GetMP() - MP);
	return true;
}

void Accessories::MakeCode() {
	Accessories::Num++;
	sprintf(Item::code, "%03d_%03d", ACCESSORIES_CODE, Accessories::Num);
}