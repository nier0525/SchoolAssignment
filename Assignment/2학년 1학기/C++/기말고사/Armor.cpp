#include "Armor.h"
#include "Character.h"

int Armor::Num = NODATA;

// ���� �ʱ�ȭ
Armor::Armor(const char* _name, int _price, int _Def) : Item(_name, _price) {
	Def = _Def;
}

Armor::~Armor() {

}

// �б� ������

int Armor::GetFirstAbility() {
	return Def;
}

int Armor::GetSecordAbility() {
	return 0;
};

// ��� �����

bool Armor::OnItem(Character* index) {
	index->SetDef(index->GetDef() + Def);
	// �ش� �������� ���ɸ�ŭ ĳ������ ���� ���
	return true;
}

// ��� ������

bool Armor::OffItem(Character* index) {
	index->SetDef(index->GetDef() - Def);
	// ������ ����� ���ɸ�ŭ ĳ���� ���� ����
	return true;
}

// ������ �������� ���ο� �ڵ� ����

void Armor::MakeCode() {
	Armor::Num++;
	sprintf(Item::code, "%03d_%03d", ARMOR_CODE, Armor::Num);
}