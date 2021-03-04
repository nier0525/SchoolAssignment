#pragma once

#include "Item.h"
#define WEAPON_CODE 201		// 아이템 코드

class Character;

class Wenpon : public Item {
private:
	// 능력치 및 일련 코드
	int Att;
	static int Num;
public:
	Wenpon(const char*, int, int); // 이름, 가격, 능력치, 능력치2
	~Wenpon();

	// 읽기 접근자
	virtual int GetFirstAbility();
	virtual int GetSecordAbility();

	// 코드 생성
	virtual void MakeCode();
	// 아이템 착용,해제 시 캐릭터 스탯 변경 담당
	virtual bool OnItem(Character*);
	virtual bool OffItem(Character*);
};