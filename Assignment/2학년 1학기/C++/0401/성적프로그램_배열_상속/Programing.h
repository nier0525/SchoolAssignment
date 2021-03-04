#pragma once

#include "Manager.h"

class CPrograming : public Manager {
private:
	static const char* SubName[];
public:
	CPrograming();
	~CPrograming();

	void input();
	void display();
	void serach();
};