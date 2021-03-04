#ifndef CMAIN_H
#define CMAIN_H
#include "Global.h"
#include "../Manager/Manager.h"
#include "../Manager/DesignManager/DesignManager.h"
#include "../Manager/GraphicManager/GraphicManager.h"
#include "../Manager/ProgramManager/ProgrammingManager.h"

class CMainManager
{
private:
	CManager*	S[4];
public:
	CMainManager();	
	~CMainManager();

	void			Initialize();
	int				ShowMenu();
	void			MajorInput();
	void			MajorSearchData();
	void			MajorDisplay();

	void			Run();

};
#endif