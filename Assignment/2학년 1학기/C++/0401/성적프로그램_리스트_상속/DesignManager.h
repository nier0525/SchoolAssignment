#pragma once

#include "DBManager.h"

class CDesign : public DBManager {
private:
	static const char* Sublist[];
public:
	CDesign();
	~CDesign();

	void input();
	void display();
	void search();
};