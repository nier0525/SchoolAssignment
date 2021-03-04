#pragma once

#include "Character.h"

class Oak : public Character {	// 캐릭터 클래스를 상속 받음
private:
	char brood[CHARSIZE];	// 종족
	char job[CHARSIZE];		// 직업
public:
	Oak(const char*);		// 생성자(상속)
	~Oak();

	// 쓰기 접근자
	void SetBrood(const char*);
	void SetJob(const char*);

	// 읽기 접근자
	const char* GetBrood();
	const char* GetJob();
};