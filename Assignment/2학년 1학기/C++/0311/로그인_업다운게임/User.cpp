#include "User.h"

CUser::CUser(const char* _id, const char* _pass) {
	strcpy(Id, _id);
	strcpy(Pass, _pass);
	uScore = 0;
}

CUser::~CUser() { }

//void CUser::Setid(const char* _id) {
//	strcpy_s(Id, _id);
//}
//
//void CUser::SetPass(const char* _pass) {
//	strcpy_s(Pass, _pass);
//}

void CUser::SetScore(const int _score) {
	if (uScore == 0 || uScore >= _score) {
		uScore = _score;
	}
	else {
		return;
	}
}

const char* CUser::Getid() {
	return Id;
}

const char* CUser::GetPass() {
	return Pass;
}

const int CUser::getScore() {
	return uScore;
}


void CUser::Login() {
	state = true;
}
void CUser::LogOut() {
	state = false;
}