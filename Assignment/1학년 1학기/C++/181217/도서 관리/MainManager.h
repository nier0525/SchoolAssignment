#ifndef MAINMANAGER_H
#define MAINMANAGER_H
#include "Base.h"
#include "BookManager.h"
#include "IDMANAGER.h"

class cMain {
	cBookManager*	mBook;
	cIDManager*		mId;
public:
	cMain();
	~cMain();

	void initialize();
	void Menu();
	void BookMenu();

};

#endif