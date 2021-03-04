#include "Item.h"

// ������ �ʱ�ȭ �۾�
Item::Item(const char* _name, int _price) {
	strcpy(name, _name);
	memset(code, 0, sizeof(code));
	price = _price;
}

Item::~Item() {

}

//----------------------------------------------

// �б� ������
const char* Item::GetItemName() {
	return name;
}

const char* Item::GetItemCode() {
	return code;
}

int Item::GetPrice() {
	return price;
}

//----------------------------------------------