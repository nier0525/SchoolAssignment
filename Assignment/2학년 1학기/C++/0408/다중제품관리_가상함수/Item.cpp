#include "Item.h"

CItem::CItem(const char* _name, const char* _makeday,const char* _mid, int _price) 
: CProduct(_name, _makeday,_mid, _price) {
	memset(id, 0, sizeof(id));
}

CItem::~CItem() {

}

void CItem::Setid(const char* _id) {
	strcpy(id, _id);
}

const char* CItem::Getid() {
	return id;
}