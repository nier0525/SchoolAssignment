#include "Book.h"

cBook::cBook(const char* _name , int _count) {
	strcpy_s(mName, _name);
	mBookCount = _count;

	mLendUserInfo = nullptr;

	mBookLendCount = 0;
	mLendIndex = 0;
	mBook = _count;
}

cBook::~cBook() {
	for (int i = 0; i < mLendIndex; i++) {
		delete mLendUserInfo[i];
	}
	delete[] mLendUserInfo;
}

void cBook::Initialize() {
	mLendUserInfo = new LendUserInfo*[mBookCount];
	MakeBookCode();
}

void cBook::MakeBookCode() {
	static int codeNumber = 0;
	sprintf(mCode, "1%02d", ++codeNumber);
}

const char* cBook::GetCode() {
	return mCode;
}

const char* cBook::GetName() {
	return mName;
}

int cBook::GetBook() {
	return mBook;
}

int cBook::GetBookCount() {
	return mBookCount;
}

int cBook::GetBookLendCount() {
	return mBookLendCount;
}

int cBook::GetLendIndex() {
	return mLendIndex;
}

void cBook::SetName(const char* _name) {
	strcpy_s(mName, _name);
}

void cBook::LendBook(const cIDMenu* _user, int _count) {
	bool flug = false;

	for (int i = 0; i < mLendIndex; i++) {
		if (mLendUserInfo[i]->user == _user) {
			flug = true;
			mLendUserInfo[i]->count += _count;
			break;
		}
	}

	if (!flug) {
		mLendUserInfo[mLendIndex] = new LendUserInfo;
		mLendUserInfo[mLendIndex]->user = _user;
		mLendUserInfo[mLendIndex]->count = _count;
		mLendIndex++;
	}
	mBookLendCount += _count;
	mBook -= _count;
}

void cBook::ReturnBook(const cIDMenu* _user, int _count) {
	for (int i = 0; i < mLendIndex; i++) {
		if (mLendUserInfo[i]->user == _user) {
			mLendUserInfo[i]->count -= _count;
			if (mLendUserInfo[i]->count == 0) {
				delete mLendUserInfo[i];
				for (int j = i; j < mLendIndex - 1; j++) {
					mLendUserInfo[j] = mLendUserInfo[j + 1];
				}
				mLendUserInfo[mLendIndex - 1] = nullptr;
				mLendIndex--;
			}
			mBook += _count;
			mBookCount -= _count;

			break;
		}
	}
}

bool cBook::CheckLendAble(int _count) {
	if (mBook < _count) {
		return false;
	}
	else {
		return true;
	}
}

bool cBook::CheckReturnAble(const cIDMenu* _user, int _count) {
	bool flug = false;
	for (int i = 0; i < mLendIndex; i++) {
		if (mLendUserInfo[i]->user == _user) {
			flug = true;
			if (mLendUserInfo[i]->count < _count) {
				return false;
			}
			else {
				return true;
			}
		}
	}
	return flug;
}