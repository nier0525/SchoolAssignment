#pragma once

#include "SchoolManager.h"

class CDesign : public CSchoolManager {
private:
	static const char* SubName[];
public:
	CDesign();
	~CDesign();

	void Input();
	void Display();
	void Search();
};