#include "Portion.h"

// �����ڿ��� �ʱ�ȭ�� ���� �θ� Ŭ�������� �Ѱ���
Portion::Portion(const char* _name, int _money) : Item(_name, _money) {
	memset(abilty, 0, sizeof(abilty));
}

Portion::~Portion() {

}

// ���� ������
void Portion::SetAblity(const char* _abilty) {
	strcpy(abilty, _abilty);
}

// �б� ������
const char* Portion::GetAblity() {
	return abilty;
}