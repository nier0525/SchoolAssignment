#include "Wear.h"

CWear::CWear(const char* _name, const char* _makeday, const char* _mid, int _price) 
: CProduct(_name, _makeday, _mid, _price) {
	memset(size, 0, sizeof(size));
}

CWear::~CWear() {

}

void CWear::Setsize(const char* _size) {
	strcpy(size, _size);
}

const char* CWear::Getsize() {
	return size;
}