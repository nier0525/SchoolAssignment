#include "Portion.h"

// 생성자에서 초기화한 값은 부모 클래스에게 넘겨줌
Portion::Portion(const char* _name, int _money) : Item(_name, _money) {
	memset(abilty, 0, sizeof(abilty));
}

Portion::~Portion() {

}

// 쓰기 접근자
void Portion::SetAblity(const char* _abilty) {
	strcpy(abilty, _abilty);
}

// 읽기 접근자
const char* Portion::GetAblity() {
	return abilty;
}