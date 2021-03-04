#include "Book.h"

cBook::cBook(const char* _name , const char* _code, int _count) {
	strcpy_s(mName, _name);
	strcpy_s(mCode, _code);
	mCount = _count;
	mBook = _count;
}

cBook::~cBook() {}

void cBook::SetName(const char* _name) {
	strcpy_s(mName, _name);
}

const char* cBook::GetName() {
	return mName;
}

void cBook::SetCode(const char* _code) {
	strcpy_s(mCode, _code);
}

const char* cBook::GetCode() {
	return mCode;
}

void cBook::SetCount(int _count) {
	mCount = _count;
}

int cBook::GetCount() {
	return mCount;
}

int cBook::GetBook() {
	return mBook;
}

