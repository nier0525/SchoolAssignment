#include "Item.h"

Item::Item(const char* _name, int _price) {
	strcpy(name, _name);
	memset(code, 0, sizeof(code));
	price = _price;
	kind = NODATA;
}

Item::~Item() {

}

//----------------------------------------------

void Item::Setkind(int _kind) {
	kind = _kind;
}

//----------------------------------------------

const char* Item::GetItemName() {
	return name;
}

const char* Item::GetItemCode() {
	return code;
}

int Item::GetPrice() {
	return price;
}

int Item::GetKind() {
	return kind;
}

//----------------------------------------------