#pragma once

#include "Manager.h"

class CProducter : public Manager {
private:
	static const char* SubName[];
public:
	CProducter();
	~CProducter();

	void input();
	void display();
	void serach();
};