#pragma once

#include "SchoolManager.h"

class CPrograming : public CSchoolManager {
private:
	static const char* SubName[];
public:
	CPrograming();
	~CPrograming();

	void Input();
	void Display();
	void Search();
};