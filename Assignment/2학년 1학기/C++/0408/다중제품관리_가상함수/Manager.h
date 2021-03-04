#pragma once

#include "Food.h"
#include "Item.h"
#include "Wear.h"

#include "ListNode.h"

class CManager {
private:

public:
	CManager() {}
	virtual ~CManager() {}

	virtual int Menu() = 0;

	virtual void init() = 0;
	virtual void Input() = 0;
	virtual void Search() = 0;
	virtual void Delete() = 0;
};