#ifndef BOOKMANAGER_H
#define BOOKMANAGER_H
#include "Base.h"
#include "Book.h"

class cBookManager {
private:
	cBook*	mBooklas[BOOKSIZE];
	int mCount;
public:
	cBookManager();
	~cBookManager();

	void Serach();
	void initelize();
	void NewBook();
	void DeleteBook();

	void Return();
	void Output();
};

#endif