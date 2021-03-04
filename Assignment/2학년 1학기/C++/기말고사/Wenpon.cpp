#include "Wenpon.h"
#include "Character.h"

int Wenpon::Num = NODATA;

// ���� �ʱ�ȭ
Wenpon::Wenpon(const char* _name, int _price, int _att) : Item(_name, _price) {
	Att = _att;
}

// �б� ������

Wenpon::~Wenpon() {

}

int Wenpon::GetFirstAbility() {
	return Att;
}

int Wenpon::GetSecordAbility() {
	return 0;
}

// ��� �����

bool Wenpon::OnItem(Character* index) {
	index->SetAtt(index->GetAtt() + Att);
	// �ش� �������� ���ɸ�ŭ ĳ������ ���� ���
	return true;
}

bool Wenpon::OffItem(Character* index) {
	index->SetAtt(index->GetAtt() - Att);
	// ������ ����� ���ɸ�ŭ ĳ���� ���� ����
	return true;
}

// ������ �������� ���ο� �ڵ� ����

void Wenpon::MakeCode() {
	Wenpon::Num++;
	sprintf(Item::code, "%03d_%03d", WEAPON_CODE, Wenpon::Num);
}