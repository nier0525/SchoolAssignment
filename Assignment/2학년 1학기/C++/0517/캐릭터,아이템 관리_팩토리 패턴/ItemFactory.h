#pragma once

#include "Wenpon.h"
#include "Armor.h"
#include "Accessories.h"
#include "ListNode.h"

class ItemFactory {
protected:
	Item* item;

	virtual Item* CreateWenpon() = 0;
	virtual Item* CreateArmor() = 0;
	virtual Item* CreateAccessories() = 0;
public:
	ItemFactory();
	virtual ~ItemFactory();
	Item* Sell(int);
	Item* GetItem();
};