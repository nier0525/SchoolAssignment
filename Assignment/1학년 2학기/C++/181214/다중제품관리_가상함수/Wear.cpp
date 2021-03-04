#include "Wear.h"


cWear::cWear() {
	memset(mWearName, 0, sizeof(mWearName));
	memset(mId, 0, sizeof(mId));
	memset(mSex, 0, sizeof(mSex));
	mSize = 0;
	mMoney = 0;
	mCount = 0;
}

cWear::~cWear() {}

const char* cWear::GetId() {
	return mId;
}
const char* cWear::GetWearName() {
	return mWearName;
}

const char* cWear::GetSex() {
	return mSex;
}

int cWear::GetSize() {
	return mSize;
}
int cWear::GetMoney() {
	return mMoney;
}
int cWear::GetCount() {
	return mCount;
}

bool cWear::SetWearName(const char* _name) {
	if (strlen(_name) > NAMESIZE) {
		return false;
	}
	strcpy(mWearName, _name);
	return true;
}

bool cWear::SetSex(const char* _name) {
	if (strlen(_name) > NAMESIZE) {
		return false;
	}
	strcpy(mSex, _name);
	return true;
}

bool cWear::SetMoney(int n) {
	if (n <= 0 || n > MAXMONEY) { return false; }
	mMoney = n;
	return true;
}

bool cWear::SetCount(int n) {
	if (n <= 0 || n > MAXCOUNT) { return false; }
	mCount = n;
	return true;
}

bool cWear::SetSize(int n) {
	if (n <= 0 || n > SIZE) { return false; }
	else {
		if (n == 1) { mSize = 90; }
		if (n == 2) { mSize = 95; }
		if (n == 3) { mSize = 100; }
		if (n == 4) { mSize = 105; }
		if (n == 5) { mSize = 110; }
		return true;
	}
}

int cWear::InCount(int n) {
	if (mCount + n > MAXCOUNT) {
		int add = MAXCOUNT - mCount;
		mCount += add;

		return add;
	}
	mCount += n;
	return n;
}

int cWear::DeCount(int n) {
	if (mCount - n <= 0) {
		int temp = mCount;
		mCount = 0;
		return temp;
	}
	mCount -= n;
	return n;
}
