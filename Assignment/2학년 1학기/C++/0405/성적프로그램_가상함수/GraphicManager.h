#pragma once

#include "SchoolManager.h"

class CGraphic : public CSchoolManager {
private:
	static const char* SubName[];
public:
	CGraphic();
	~CGraphic();

	void Input();
	void Display();
	void Search();
};