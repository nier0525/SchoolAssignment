#include "Wenpon.h"

// �����ڿ��� �ʱ�ȭ�� ���� �θ� Ŭ�������� �Ѱ���
Wenpon::Wenpon(const char* _name, int _money) : Item(_name, _money) {
	memset(abilty, 0, sizeof(abilty));
}

Wenpon::~Wenpon() {

}

// ���� ������
void Wenpon::SetAblity(const char* _abilty) {
	strcpy(abilty, _abilty);
}

// �б� ������
const char* Wenpon::GetAblity() {
	return abilty;
}