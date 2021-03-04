#include "Item.h"

// 생성자 초기화 작업
Item::Item(const char* _name, int _money) {
	strcpy(name, _name);
	money = _money;
	count = NODATA;
}

Item::~Item() {

}

// 쓰기 접근자
void Item::SetName(const char* _name) {
	strcpy(name, _name);
}

void Item::SetMoney(int _money) {
	money = _money;
}

void Item::SetCount(int _count) {
	count = _count;
}

// 읽기 접근자
const char* Item::GetName() {
	return name;
}

int Item::GetMoney() {
	return money;
}

int Item::GetCount() {
	return count;
}