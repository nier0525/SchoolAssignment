#pragma once

#include "Manager.h"

class CGraphic : public Manager {
private:
	static const char* SubName[];
public:
	CGraphic();
	~CGraphic();

	void input();
	void display();
	void serach();
};