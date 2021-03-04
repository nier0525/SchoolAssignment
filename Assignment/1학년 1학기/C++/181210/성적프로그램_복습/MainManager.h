#ifndef CMAIN_H
#define CMAIN_H
#include "Base.h";
#include "DesighManager.h"
#include "GraphicManager.h"
#include "ProgrammingManager.h"

class cMainManager {
private:
	cDesignManager*	mDesignMng;
	cGraphicManager* mGraphicMng;
	cProgrammingManager* mProgrammingMng;
public:
	cMainManager();
	~cMainManager();

	void Initialize();
	int ShowMenu();
	void MajorInput();
	void MajorSearchData();
	void MajorDisplay();

	void run();
};

#endif