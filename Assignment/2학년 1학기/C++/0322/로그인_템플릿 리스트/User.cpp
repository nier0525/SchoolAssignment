#include "User.h"

CUser::CUser() {
	memset(Id, 0, sizeof(Id));
	memset(Pass,0,sizeof(Pass));
}

CUser::~CUser() {}

void CUser::Setid(const char* _id) {
	strcpy(Id, _id);
}

void CUser::SetPass(const char* _pass) {
	strcpy(Pass, _pass);
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