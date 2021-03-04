#ifndef BOOKMANAGER_H
#define BOOKMANAGER_H
#include "Base.h"
#include "Book.h"

class cBookManager {
private:
	cBook*	mBooklas[BOOKSIZE];
	int mCount;
	
	void Host();
	void NewBook();
	void DeleteBook();
public:
	cBookManager();
	~cBookManager();

	void initelize();
	void Display();
	void Serach();
	void Return(cIDMenu*);
	void Output(cIDMenu*);

	void HostMode();
};

#endif