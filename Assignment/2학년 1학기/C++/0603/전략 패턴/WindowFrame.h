#ifndef _CWINDOWFRAME_H
#define _CWINDOWFRAME_H
#include "Global.h"
#include "Menu.h"

class CWindowFrame
{
	static CWindowFrame* mPthis;

	HWND				mhWnd;	
	HINSTANCE			mhInstance;	
	CMenu*				mMenu;				

	CWindowFrame(){}
	~CWindowFrame(){}

public:
	static CWindowFrame* Create(HINSTANCE _hinstance);
	static CWindowFrame* GetInstance();
	static void Destroy();

	static LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam);

	void Run(MSG _message);
	void Initialize();
	void BuildWindow();
	HWND GetHWnd(){ return mhWnd; }
};
#endif
