#include "IDMenu.h"

cIDMenu::cIDMenu(const char* _id, const char* _pass) {
	strcpy_s(mId, _id);
	strcpy_s(mPass, _pass);

	memset(mBorrowInfo, 0, sizeof(mBorrowInfo));
	mBcount = 0;
	mBindex = 0;
	mMaxBorrow = BORROW_MAX;
}

cIDMenu::~cIDMenu() {
	for (int i = 0; i < mBindex; i++) {
		delete mBorrowInfo[i];
	}
}

void cIDMenu::SetId(const char* _id) {
	strcpy_s(mId, _id);
}
void cIDMenu::SetPass(const char* _pass) {
	strcpy_s(mPass, _pass);
}

const char* cIDMenu::GetId() {
	return mId;
}

const char*	cIDMenu::GetPass() {
	return mPass;
}

int cIDMenu::GetBorrowCount() {
	return mBcount;
}

void cIDMenu::BorrowBook(const char* _code, int _count) {
	bool flug = false;
	for (int i = 0; i < mBindex; i++) {
		if (!strcmp(mBorrowInfo[i]->code, _code)) {
			mBorrowInfo[i]->count += _count;
			flug = true;
			break;
		}
	}
	if (!flug) {
		mBorrowInfo[mBindex] = new Book_Borrow_Info;
		memset(mBorrowInfo[mBindex], 0, sizeof(Book_Borrow_Info));
		strcpy_s(mBorrowInfo[mBindex]->code, _code);
		mBorrowInfo[mBindex]->count = _count;
		mBindex++;
	}
	mBcount += _count;
}

void cIDMenu::ReturnBook(const char* _code, int _count) {
	for (int i = 0; i < mBindex; i++) {
		if (!strcmp(mBorrowInfo[i]->code, _code)) {
			mBorrowInfo[i]->count -= _count;
			mBcount -= _count;
			
			if (mBorrowInfo[i]->count == 0) {
				delete mBorrowInfo[i];

				for (int j = i; j < mBindex - 1; j++) {
					mBorrowInfo[j] = mBorrowInfo[j + 1];
				}
				mBorrowInfo[mBindex - 1] = nullptr;
				mBindex--;
			}
			break;
		}
	}
}

bool cIDMenu::CheckBorrowAble(int _count) {
	if (mBcount + _count > BORROW_MAX) {
		return false;
	}
	return true;
}

bool cIDMenu::CheckReturnAble(const char* _code, int _count) {
	bool flug = false;

	for (int i = 0; i < mBindex; i++) {
		if (!strcmp(mBorrowInfo[i]->code, _code)) {
			flug = true;
			if (mBorrowInfo[i]->count >= _count) {
				return true;
			}
			else {
				return false;
			}
		}
	}
	return flug;
}

void cIDMenu::Login() {
	mLoginState = true;
}
void cIDMenu::Logout() {
	mLoginState = false;
}