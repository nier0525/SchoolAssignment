#include "Item.h"

cItem::cItem() {
	memset(mItemName, 0, sizeof(mItemName));
	memset(mId, 0, sizeof(mId));
	mItemid = 0;
	mFreeAs = 0;
	mMoney = 0;
	mCount = 0;
}

cItem::~cItem() {}

const char* cItem::GetId() {
	return mId;
}
const char* cItem::GetItemName() {
	return mItemName;
}
int cItem::GetItemid() {
	return mItemid;
}
int cItem::GetFreeAs() {
	return mFreeAs;
}
int cItem::GetMoney() {
	return mMoney;
}
int cItem::GetCount() {
	return mCount;
}

bool cItem::SetItemName(const char* _name) {
	if (strlen(_name) > NAMESIZE) {
		return false;
	}
	strcpy(mItemName, _name);
	return true;
}

bool cItem::SetMoney(int n) {
	if (n <= 0 || n > MAXMONEY) { return false; }
	mMoney = n;
	return true;
}

bool cItem::SetCount(int n) {
	if (n <= 0 || n > MAXCOUNT) { return false; }
	mCount = n;
	return true;
}

bool cItem::SetItemid(int n) {
	if (n <= 0 || n > IDSIZE) { return false; }
	mItemid = n;
	return true;
}

bool cItem::SetFreeAs(int n) {
	if (n <= 0 || n > 25) { return false; }
	mFreeAs = n;
	return true;
}

int cItem::InCount(int n) {
	if (mCount + n > MAXCOUNT) {
		int add = MAXCOUNT - mCount;
		mCount += add;

		return add;
	}
	mCount += n;
	return n;
}

int cItem::DeCount(int n) {
	if (mCount - n <= 0) {
		int temp = mCount;
		mCount = 0;
		return temp;
	}
	mCount -= n;
	return n;
}
