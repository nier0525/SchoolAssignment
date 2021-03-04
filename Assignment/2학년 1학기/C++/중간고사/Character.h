#pragma once

#include "Global.h"

class Character {
private:
	char name[CHARSIZE];	// 닉네임
	char sex[CHARSIZE];		// 성별
public:
	Character(const char*);	// 생성자 (닉네임)
	~Character();			// 소멸자

	// 쓰기 접근자
	void SetName(const char*);
	void SetSex(const char*);
	// 읽기 접근자
	const char* GetName();
	const char* GetSex();

	// 자식들의 함수를 읽기 위한 순수 가상 함수
	virtual void SetBrood(const char*) = 0;
	virtual void SetJob(const char*) = 0;
	virtual const char* GetBrood() = 0;
	virtual const char* GetJob() = 0;
};