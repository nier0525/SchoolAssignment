#ifndef IDMANAGER_H
#define IDMANAGER_H
#include "Base.h"
#include "IDMenu.h"

class cIDManager {
private:
	cIDMenu*	mIDlat[MAXID];
	int mCount;
public:
	cIDManager();
	~cIDManager();

	void New();
	bool Rogin();
	void DeleteRog();

};

#endif