#include "Armor.h"
#include "Character.h"

int Armor::Num = NODATA;

Armor::Armor(const char* _name, int _price, int _Def) : Item(_name, _price) {
	Def = _Def;
	Item::Setkind(ARMOR);
}

Armor::~Armor() {

}

int Armor::GetFirstAbility() {
	return Def;
}

int Armor::GetSecordAbility() {
	return 0;
};

bool Armor::OnItem(Character* index) {
	index->SetDef(index->GetDef() + Def);
	return true;
}

bool Armor::OffItem(Character* index) {
	index->SetDef(index->GetDef() - Def);
	return true;
}

void Armor::MakeCode() {
	Armor::Num++;
	sprintf(Item::code, "%03d_%03d", ARMOR_CODE, Armor::Num);
}