#pragma once

#include "Global.h"

class Item {
private:
	char name[CHARSIZE];	// 아이템 이름
	int money;				// 아이템 가격
	int count;				// 아이템 개수
public:
	Item(const char*, int);	// 생성자 (이름, 가격)
	~Item();

	// 쓰기 접근자
	void SetName(const char*);
	void SetCount(int);
	void SetMoney(int);

	// 읽기 접근자
	const char* GetName();
	int GetCount();
	int GetMoney();

	// 자식 내에 함수를 읽기 위한 순수 가상 함수
	virtual void SetAblity(const char*) = 0;
	virtual const char* GetAblity() = 0;
};