#ifndef ITEMMANAGER_H
#define ITEMMANAGER_H
#include "Item.h"
#include "Manager.h"

class cItemManager : public cManager {
private:
	static const char*	mItemName[];
	cItem*	mItem[MAXSIZE];
	int mItemCount;
public:
	cItemManager();
	~cItemManager();

	void Input();
	void Output();
	void Show();
};

#endif