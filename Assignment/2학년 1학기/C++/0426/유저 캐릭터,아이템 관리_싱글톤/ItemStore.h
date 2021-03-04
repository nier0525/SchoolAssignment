#pragma once

#include "LoginManager.h"
#include "Wenpon.h"
#include "Armor.h"
#include "Accessories.h"

class ItemStore {
private:
	static const char* ItemName[];

	ListNode<Item*>* ItemList;
	Node<Item*>* SearchItem;

	static ItemStore* pthis;
	ItemStore();
	~ItemStore();
public:
	static ItemStore* GetInstance();
	static void Destory();

	int Menu();

	void Init();

	void Buy();
	void Sell();
};