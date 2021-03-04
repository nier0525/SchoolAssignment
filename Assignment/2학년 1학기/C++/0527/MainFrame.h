#ifndef _CMAINFRAME_H
#define _CMAINFRAME_H
#include "WindowFrame.h"

#include <windows.h>

class CMainFrame
{
	static CMainFrame* mPthis;

	CMainFrame(){}
	~CMainFrame(){}

public:
	static CMainFrame* Create(HINSTANCE _hinstance);
	static CMainFrame* GetInstance();
	static void Destroy();

	void Initialize();
	int Run();


};

#endif
