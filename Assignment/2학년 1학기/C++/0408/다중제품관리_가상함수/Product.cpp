#include "Product.h"

CProduct::CProduct(const char* _name, const char* _makeday, const char* _mid, int _price) {
	strcpy(name, _name);
	strcpy(makeday, _makeday);
	strcpy(mid, _mid);
	price = _price;
}

CProduct::~CProduct() {

}

void CProduct::Setname(const char* _name) {
	strcpy(name, _name);
}

void CProduct::Setmakeday(const char* _makeday) {
	strcpy(makeday, _makeday);
}

void CProduct::Setprice(int _price) {
	price = _price;
}

void CProduct::Setmid(const char* _mid) {
	strcpy(mid, _mid);
}

const char* CProduct::Getname() {
	return name;
}

const char* CProduct::Getmakeday() {
	return makeday;
}

int CProduct::Getprice() {
	return price;
}

const char* CProduct::Getmid() {
	return mid;
}