#include "Character.h"

// 생성자 초기화 작업
Character::Character(const char* _name) {
	strcpy(name, _name);
	memset(sex, 0, sizeof(sex));
}

Character::~Character() {

}

// 쓰기 접근자
void Character::SetName(const char* _name) {
	strcpy(name, _name);
}

void Character::SetSex(const char* _sex) {
	strcpy(sex, _sex);
}

// 읽기 접근자
const char* Character::GetName() {
	return name;
}

const char* Character::GetSex() {
	return sex;
}

