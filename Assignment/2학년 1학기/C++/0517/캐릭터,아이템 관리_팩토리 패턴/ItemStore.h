#pragma once

#include "Wenpon.h"
#include "Armor.h"
#include "Accessories.h"

class ItemStore {
private:
	static ItemStore* pthis;
	ItemStore();
	~ItemStore();
public:
	static ItemStore* GetInstance();
	static void Destory();

	void Buy();
	void Sell();
};