#include "Accessories.h"
#include "Character.h"

int Accessories::Num = NODATA;

// ���� �ʱ�ȭ
Accessories::Accessories(const char* _name, int _price, int _HP, int _MP) : Item(_name, _price) {
	HP = _HP;
	MP = _MP;
}

Accessories::~Accessories() {

}

// �б� ������

int Accessories::GetFirstAbility() {
	return HP;
}

int Accessories::GetSecordAbility() {
	return MP;
}

// ��� �����

bool Accessories::OnItem(Character* index) {
	index->SetHp(index->GetHp() + HP);
	index->SetMp(index->GetMp() + MP);
	// �ش� �������� ���ɸ�ŭ ĳ������ ���� ���
	return true;
}

// ��� ������

bool Accessories::OffItem(Character* index) {
	index->SetHp(index->GetHp() - HP);
	index->SetMp(index->GetMp() - MP);
	// ������ ����� ���ɸ�ŭ ĳ���� ���� ����
	return true;
}

// ������ �������� ���ο� �ڵ� ����

void Accessories::MakeCode() {
	Accessories::Num++;
	sprintf(Item::code, "%03d_%03d", ACCESSORIES_CODE, Accessories::Num);
}