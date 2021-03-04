#ifndef IDMANAGER_H
#define IDMANAGER_H
#include "Base.h"
#include "IDMenu.h"

class cIDManager {
private:
	cIDMenu*	mIDlat[MAXID];
	int mCount;

	cIDMenu*	mNowUser;
public:
	cIDManager();
	~cIDManager();

	cIDMenu*	GetNowUser();

	void New();
	bool Rogin();
	void Rogout();
	void DeleteRog();

};

#endif