#ifndef CGRAPHIC_MANAGER_H
#define CGRAPHIC_MANAGER_H
#include "../../Student/Student.h"
#include "../Manager.h"

class CGraphicManager : public CManager
{
private:
	static const char*			mSubjectName[];

public:
	CGraphicManager();
	~CGraphicManager();

	void Input();
	void Display();
	void Search();

};
#endif