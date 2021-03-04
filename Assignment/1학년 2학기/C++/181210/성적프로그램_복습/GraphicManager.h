#ifndef CGRAPHIC_MANAGER_H
#define CGRAPHIC_MANAGER_H
#include "Student.h"
#include "Manager.h"

class cGraphicManager : public cManager {
private:
	static const char*	mSubjectName[];
public:
	cGraphicManager();
	~cGraphicManager();

	void Input();
	void Display();
	void Search();
};

#endif