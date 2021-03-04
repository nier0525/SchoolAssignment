#ifndef CMANAGER_H
#define CMANAGER_H

#include "Product.h"

class cManager {
private:
	static const char* mNameList[];
	cProduct* ListProduct[LISTMAX];
	cProduct* product;
	int mListCount;
public:
	cManager();
	~cManager();

	void Add();
	void Del();
	void Display();

	void CinCheak();
	void Run();
};

#endif