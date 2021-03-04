#pragma once

#include "ItemFactory.h"

class ArcherItem : public ItemFactory {
private:
	ListNode<Item*>* ArcherItemLIst;
	Node<Item*>* SelectItem;

	virtual Item* CreateWenpon();
	virtual Item* CreateArmor();
	virtual Item* CreateAccessories();

	static ArcherItem* pthis;
	ArcherItem();
	~ArcherItem();
public:
	static ArcherItem* GetInstance();
	static void Destory();
};