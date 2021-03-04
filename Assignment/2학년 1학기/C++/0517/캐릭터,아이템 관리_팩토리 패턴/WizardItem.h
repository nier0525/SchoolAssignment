#pragma once

#include "ItemFactory.h"

class WizardItem : public ItemFactory {
private:
	ListNode<Item*>* WizardItemLIst;
	Node<Item*>* SelectItem;

	virtual Item* CreateWenpon();
	virtual Item* CreateArmor();
	virtual Item* CreateAccessories();

	static WizardItem* pthis;
	WizardItem();
	~WizardItem();
public:
	static WizardItem* GetInstance();
	static void Destory();
};