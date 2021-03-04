#pragma once

#include "DBManager.h"

class CPrograming : public DBManager {
private:
	static const char* Sublist[];
public:
	CPrograming();
	~CPrograming();

	void input();
	void display();
	void search();
};