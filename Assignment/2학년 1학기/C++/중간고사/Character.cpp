#include "Character.h"

// ������ �ʱ�ȭ �۾�
Character::Character(const char* _name) {
	strcpy(name, _name);
	memset(sex, 0, sizeof(sex));
}

Character::~Character() {

}

// ���� ������
void Character::SetName(const char* _name) {
	strcpy(name, _name);
}

void Character::SetSex(const char* _sex) {
	strcpy(sex, _sex);
}

// �б� ������
const char* Character::GetName() {
	return name;
}

const char* Character::GetSex() {
	return sex;
}

