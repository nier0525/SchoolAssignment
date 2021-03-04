#include "IDMenu.h"

cIDMenu::cIDMenu(const char* _id, const char* _pass) {
	strcpy_s(mId, _id);
	strcpy_s(mPass, _pass);
}

cIDMenu::~cIDMenu() {}

void cIDMenu::SetId(const char* _id) {
	strcpy_s(mId, _id);
}
void cIDMenu::SetPass(const char* _pass) {
	strcpy_s(mPass, _pass);
}

const char* cIDMenu::GetId() {
	return mId;
}

const char*	cIDMenu::GetPass() {
	return mPass;
}