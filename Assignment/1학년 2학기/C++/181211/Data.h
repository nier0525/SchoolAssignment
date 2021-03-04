#ifndef CDATA_H
#define CDATA_H
#include "Base.h"

class cFruit {
private:
	int mCount;
	int mMoney; // 멤버 변수
public:
	cFruit(); // 생성자
	~cFruit(); // 소멸자

	void SetCount(int);
	void SetMoney(int);

	int GetCount();
	int GetMoney();
	// 함수
};

#endif