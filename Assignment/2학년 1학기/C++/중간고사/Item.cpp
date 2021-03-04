#include "Item.h"

// ������ �ʱ�ȭ �۾�
Item::Item(const char* _name, int _money) {
	strcpy(name, _name);
	money = _money;
	count = NODATA;
}

Item::~Item() {

}

// ���� ������
void Item::SetName(const char* _name) {
	strcpy(name, _name);
}

void Item::SetMoney(int _money) {
	money = _money;
}

void Item::SetCount(int _count) {
	count = _count;
}

// �б� ������
const char* Item::GetName() {
	return name;
}

int Item::GetMoney() {
	return money;
}

int Item::GetCount() {
	return count;
}