#ifndef MAINMANAGER_H
#define MAINMANAGER_H
#include "BookManager.h"
#include "IDMANAGER.h"

class cMain {
	static char _ID[];
	static char _PASS[];
	cBookManager*	mBook;
	cIDManager*		mId;
	bool type = true;
public:
	cMain();
	~cMain();

	void initialize();

	void Menu();
	void ManagerID();
	void BookMenu();

};

#endif