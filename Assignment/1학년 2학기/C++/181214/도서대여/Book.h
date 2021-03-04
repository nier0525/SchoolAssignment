#ifndef BOOK_H
#define BOOK_H
#include "Base.h"

class cBook {
private:
	char mName[BOOKNAME];
	char mCode[BOOKCODE];
	int mCount;
	int mBook;
public:
	cBook(const char* , const char* ,int);
	~cBook();

	void SetName(const char*);
	const char* GetName();

	void SetCode(const char*);
	const char*	GetCode();
	
	void SetCount(int);
	int GetCount();

	int GetBook();
};

#endif
