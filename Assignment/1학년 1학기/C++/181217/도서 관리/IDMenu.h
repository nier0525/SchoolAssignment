#ifndef IDMENU_H
#define IDMENU_H
#include "Base.h"

struct Book_Borrow_Info {
	char code[BOOKCODE];
	int count;
};

class cIDMenu {
private:
	char mId[ID];
	char mPass[PASSWORD];

	int	mMaxBorrow;
	Book_Borrow_Info*	mBorrowInfo[BORROW_MAX];
	int		mBcount;
	int		mBindex;
	bool	mLoginState;

public:
	cIDMenu(const char*, const char*);
	~cIDMenu();

	const char* GetId();
	const char* GetPass();
	int GetBorrowCount();

	void SetId(const char*);
	void SetPass(const char*);

	void BorrowBook(const char*, int);
	void ReturnBook(const char*, int);
	void Login();
	void Logout();

	bool	CheckBorrowAble(int);
	bool	CheckReturnAble(const char*, int);
};

#endif