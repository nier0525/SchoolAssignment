#pragma once

#include "Manager.h"

class CWearManager : public CManager {
private:
	static const char* WearName[];
	ListNode<CWear*>* ProductList[3];
	Node<CWear*>* NowProduct;
public:
	CWearManager();
	~CWearManager();
	
	int Menu();

	void init();
	void Input();
	void Search();
	void Delete();
};