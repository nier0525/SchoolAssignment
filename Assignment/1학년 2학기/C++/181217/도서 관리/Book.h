#ifndef BOOK_H
#define BOOK_H
#include "Base.h"
#include "IDMenu.h"
struct LendUserInfo {
	const cIDMenu*	user;
	int count;
};

class cBook {
private:
	char mName[BOOKNAME];
	char mCode[BOOKCODE];
	int mBookCount;
	int mBookLendCount;
	LendUserInfo**	mLendUserInfo;
	int mLendIndex;
	int mBook;

public:
	cBook(const char*,int);
	~cBook();

	const char* GetName();
	const char*	GetCode();
	int GetBook();
	int GetBookCount();
	int GetBookLendCount();
	int GetLendIndex();

	void SetName(const char*);
	void MakeBookCode();

	void LendBook(const cIDMenu*, int);
	void ReturnBook(const cIDMenu*, int);
	bool CheckLendAble(int);
	bool CheckReturnAble(const cIDMenu*, int);

	void Initialize();
};

#endif
