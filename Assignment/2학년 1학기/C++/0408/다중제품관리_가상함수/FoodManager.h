#pragma once

#include "Manager.h"

class CFoodManager : public CManager {
private:
	static const char* FoodName[];
	ListNode<CFood*>* ProductList[5];
	Node<CFood*>* NowProduct;
public:
	CFoodManager();
	~CFoodManager();

	int Menu();
	
	void init();
	void Input();
	void Search();
	void Delete();
};