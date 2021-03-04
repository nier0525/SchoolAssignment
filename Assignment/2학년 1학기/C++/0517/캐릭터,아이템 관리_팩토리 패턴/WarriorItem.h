#pragma once

#include "ItemFactory.h"

class WarriorItem : public ItemFactory {
private:
	ListNode<Item*>* WarriorItemLIst;
	Node<Item*>* SelectItem;

	virtual Item* CreateWenpon();
	virtual Item* CreateArmor();
	virtual Item* CreateAccessories();

	static WarriorItem* pthis;
	WarriorItem();
	~WarriorItem();
public:
	static WarriorItem* GetInstance();
	static void Destory();
};