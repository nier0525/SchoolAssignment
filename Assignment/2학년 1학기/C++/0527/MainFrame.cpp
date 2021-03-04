#include "MainFrame.h"
CMainFrame* CMainFrame::mPthis = NULL;

CMainFrame* CMainFrame::Create(HINSTANCE _hinstance)
{
	if (!mPthis)
	{
		mPthis = new CMainFrame();
	}

	CWindowFrame::Create(_hinstance);

	return mPthis;
}
CMainFrame* CMainFrame::GetInstance()
{
	return mPthis;

}
void CMainFrame::Destroy()
{
	CWindowFrame::Destroy();

	if (mPthis)
	{
		delete mPthis;
		mPthis = NULL;
	}

	
}

void CMainFrame::Initialize()
{
	CWindowFrame::GetInstance()->BuildWindow();
}

int CMainFrame::Run()
{
	MSG Message;	

	while (GetMessage(&Message, NULL, 0, 0))
	{
		CWindowFrame::GetInstance()->Run(Message);
		
	}

	return Message.wParam;

}