#include "Data.h"

cFruit::cFruit() {
	mMoney = 0;	mCount = 0; // 멤버 변수 값 초기화
}

cFruit::~cFruit() {}

void cFruit::SetCount(int _Count) {
	mCount = _Count; // 외부의 int 값을 저장
}

void cFruit::SetMoney(int _Money) {
	mMoney = _Money; // 외부의 int 값을 저장
}

int cFruit::GetCount() {
	return mCount; // 변수 값 리턴
}

int cFruit::GetMoney() {
	return mMoney; // 변수 값 리턴
}