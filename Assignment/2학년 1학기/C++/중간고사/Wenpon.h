#pragma once

#include "Item.h"

class Wenpon : public Item {	// 아이템 클래스 상속
private:
	char abilty[CHARSIZE]; // 아이템 성능
public:
	Wenpon(const char*, int);	// 생성자 (상속)
	~Wenpon();

	// 쓰기 접근자
	void SetAblity(const char*);

	// 읽기 접근자
	const char* GetAblity();
};