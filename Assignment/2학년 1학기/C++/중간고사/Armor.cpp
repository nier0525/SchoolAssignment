#include "Armor.h"

// 생성자에서 초기화한 값은 부모 클래스에게 넘겨줌
Armor::Armor(const char* _name, int _money) : Item(_name, _money) {
	memset(abilty, 0, sizeof(abilty));
}

Armor::~Armor() {

}

// 쓰기 접근자
void Armor::SetAblity(const char* _abilty) {
	strcpy(abilty, _abilty);
}

// 읽기 접근자
const char* Armor::GetAblity() {
	return abilty;
}