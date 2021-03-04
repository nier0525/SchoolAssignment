#include "Wenpon.h"

// 생성자에서 초기화된 값은 부모 클래스에게 넘겨줌
Wenpon::Wenpon(const char* _name, int _money) : Item(_name, _money) {
	memset(abilty, 0, sizeof(abilty));
}

Wenpon::~Wenpon() {

}

// 쓰기 접근자
void Wenpon::SetAblity(const char* _abilty) {
	strcpy(abilty, _abilty);
}

// 읽기 접근자
const char* Wenpon::GetAblity() {
	return abilty;
}