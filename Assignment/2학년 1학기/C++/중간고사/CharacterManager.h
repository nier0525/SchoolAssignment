#pragma once

#include "Elf.h"
#include "Human.h"
#include "Oak.h"

#include "User.h"

// 캐릭터 생성,삭제를 담당하는 매니저 클래스
// 상위 매니저 클래스에게 현재 접속중인 유저 값을 받아서
// 그 유저가 가진 캐릭터 리스트에 캐릭터를 생성 또는 삭제하는 연산은 담당함
class CharacterManager {
private:
	static const char* brood[];	// 종족
	static const char* sex[];	// 성별

	static const char* Elfjob[];	// 엘프 직업군
	static const char* Oakjob[];	// 오크 직업군
	static const char* Humanjob[];	// 휴먼 직업군
public:
	CharacterManager();
	~CharacterManager();

	int broodMenu();	// 종족 메뉴
	const char* sexMenu();	// 성별 메뉴
	const char* jobMenu(int);	// 직업 메뉴

	void New(Node<CUser*>*); // 캐릭터 생성 함수
	void Del(Node<CUser*>*); // 캐릭터 삭제 함수
};