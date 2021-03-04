#include "Food.h"

cFood::cFood() {
	memset(mFoodName, 0, sizeof(mFoodName));
	memset(mId, 0, sizeof(mId));
	mRank = 0;
	mGram = 0;
	mMoney = 0;
	mCount = 0;
}

cFood::~cFood() {}

const char* cFood::GetId() {
	return mId;
}
const char* cFood::GetFoodName() {
	return mFoodName;
}
int cFood::GetRank() {
	return mRank;
}
int cFood::GetGram() {
	return mGram;
}
int cFood::GetMoney() {
	return mMoney;
}
int cFood::GetCount() {
	return mCount;
}

bool cFood::SetFoodName(const char* _name) {
	if (strlen(_name) > NAMESIZE) {
		return false;
	}
	strcpy(mFoodName, _name);
	return true;
}

bool cFood::SetMoney(int n) {
	if (n <= 0 || n > MAXMONEY) { return false; }
	mMoney = n;
	return true;
}

bool cFood::SetCount(int n) {
	if (n <= 0 || n > MAXCOUNT) { return false; }
	mCount = n;
	return true;
}

bool cFood::SetGram(int n) {
	if (n <= 0 || n > MAXGRAM) { return false; }
	mGram = n;
	return true;
}

bool cFood::SetRand(int n) {
	if (n <= 0 || n > RANK) { return false; }
	mRank = n;
	return true;
}

int cFood::InCount(int n) {
	if (mCount + n > MAXCOUNT) {
		int add = MAXCOUNT - mCount;
		mCount += add;

		return add;
	}
	mCount += n;
	return n;
}

int cFood::DeCount(int n) {
	if (mCount - n <= 0) {
		int temp = mCount;
		mCount = 0;
		return temp;
	}
	mCount -= n;
	return n;
}
