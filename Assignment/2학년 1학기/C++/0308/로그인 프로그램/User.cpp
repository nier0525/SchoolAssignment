#include "User.h"

CUser::CUser(const char* _id, const char* _pass) {
	strcpy(Id, _id);
	strcpy(Pass, _pass);
}

CUser::~CUser() { }

void CUser::Setid(const char* _id) {
	strcpy_s(Id, _id);
}

void CUser::SetPass(const char* _pass) {
	strcpy_s(Pass, _pass);
}

const char* CUser::Getid() {
	return Id;
}

const char* CUser::GetPass() {
	return Pass;
}

void CUser::Login() {
	state = true;
}
void CUser::LogOut() {
	state = false;
}