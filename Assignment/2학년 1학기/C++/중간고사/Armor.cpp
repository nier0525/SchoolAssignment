#include "Armor.h"

// �����ڿ��� �ʱ�ȭ�� ���� �θ� Ŭ�������� �Ѱ���
Armor::Armor(const char* _name, int _money) : Item(_name, _money) {
	memset(abilty, 0, sizeof(abilty));
}

Armor::~Armor() {

}

// ���� ������
void Armor::SetAblity(const char* _abilty) {
	strcpy(abilty, _abilty);
}

// �б� ������
const char* Armor::GetAblity() {
	return abilty;
}