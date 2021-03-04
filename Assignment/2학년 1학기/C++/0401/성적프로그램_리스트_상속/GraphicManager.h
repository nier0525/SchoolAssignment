#pragma once

#include "DBManager.h"

class CGraphic : public DBManager {
private:
	static const char* Sublist[];
public:
	CGraphic();
	~CGraphic();

	void input();
	void display();
	void search();
};