#include "Wenpon.h"
#include "Character.h"

int Wenpon::Num = NODATA;

// 변수 초기화
Wenpon::Wenpon(const char* _name, int _price, int _att) : Item(_name, _price) {
	Att = _att;
}

// 읽기 접근자

Wenpon::~Wenpon() {

}

int Wenpon::GetFirstAbility() {
	return Att;
}

int Wenpon::GetSecordAbility() {
	return 0;
}

// 장비 착용시

bool Wenpon::OnItem(Character* index) {
	index->SetAtt(index->GetAtt() + Att);
	// 해당 아이템의 성능만큼 캐릭터의 스탯 향상
	return true;
}

bool Wenpon::OffItem(Character* index) {
	index->SetAtt(index->GetAtt() - Att);
	// 해제한 장비의 성능만큼 캐릭터 스탯 감소
	return true;
}

// 아이템 생성마다 새로운 코드 생성

void Wenpon::MakeCode() {
	Wenpon::Num++;
	sprintf(Item::code, "%03d_%03d", WEAPON_CODE, Wenpon::Num);
}