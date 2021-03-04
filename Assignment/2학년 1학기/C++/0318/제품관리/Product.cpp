#include "Product.h"

cProduct::cProduct() {
	memset(mProductName, 0, sizeof(mProductName));
	memset(mID, 0, sizeof(mID));
	mCount = 0;
	mMoney = 0;
}

cProduct::~cProduct() {

}



int cProduct::GetCount() {
	return mCount;
}

int cProduct::GetMoney() {
	return mMoney;
}

const char* cProduct::GetName() {
	return mProductName;
}

const char* cProduct::GetID() {
	return mID;
}

void cProduct::MakeID() {
	
}

bool cProduct::SetCount(int _count) {
	if (_count < 0 || _count > SIZEMAX) {
		return false;
	}
	mCount = _count;
	return true;
}

bool cProduct::SetName(const char* _name) {
	if (strlen(_name) > NAMESIZE) {
		return false;
	}
	strcpy_s(mProductName, _name);
	return true;
}

bool cProduct::SetMoney(int _money) {
	if (_money < 0 || _money > MONEYMAX) {
		return false;
	}
	mMoney = _money;
	return true;
}

int cProduct::Add(int _count) {
	if (mCount + _count > SIZEMAX) { // 현재 개수 + 입고 개수가 최대 개수를 초과 할 시
		int index = SIZEMAX - mCount; // 지역 변수 index 에 최대 개수 - 현재 개수의 값 저장
		mCount += index; // 그리고 남은 값은 멤버 변수에 대입 ( 결과적으로 10개 )
		return index; // 대입한 값을 리턴
	}
	mCount += _count;
	return _count;
}

int cProduct::Delete(int _count) {
	if (mCount - _count <= 0) { // 현재 개수 - 입고 개수가 0 보다 작거나 같을 시
		int index = mCount; // 지역 변수 index 에 현재 멤버 변수가 가진 정수 값을 저장
		mCount = 0; // 그리고 멤버 변수 값 0 으로 초기화
		return index; // index 에 저장되어 있는 값을 리턴
	}
	mCount -= _count;
	return _count;
}