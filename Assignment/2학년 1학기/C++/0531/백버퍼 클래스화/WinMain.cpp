#include "MainFrame.h"

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
		  ,LPSTR lpszCmdParam,int nCmdShow)
{
	CMainFrame::Create(hInstance);
	CMainFrame::GetInstance()->Initialize();
	int result = CMainFrame::GetInstance()->Run();
	CMainFrame::GetInstance()->Destroy();

	return result;
}