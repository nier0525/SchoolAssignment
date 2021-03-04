#pragma once

#include "Manager.h"

class CItemManager : public CManager {
private:
	static const char* ItemName[];
	ListNode<CItem*>* ProductList[4];
	Node<CItem*>* NowProduct;
public:
	CItemManager();
	~CItemManager();

	int Menu();

	void init();
	void Input();
	void Search();
	void Delete();
};