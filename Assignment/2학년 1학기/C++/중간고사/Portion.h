#pragma once

#include "Item.h"

class Portion : public Item {	// 아이템 클래스를 상속
private:
	char abilty[CHARSIZE];	// 아이템 성능
public:
	Portion(const char*, int);	// 생성자 (상속)
	~Portion();

	// 쓰기 접근자
	void SetAblity(const char*);
	// 읽기 접근자
	const char* GetAblity();
};