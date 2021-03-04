#ifndef _CWINDOWFRAME_H
#define _CWINDOWFRAME_H
#include <windows.h>

class CWindowFrame
{
	static CWindowFrame* mPthis;

	HWND				mhWnd;
	//MSG mMessage;
	//WNDCLASS mWndClass;
	HINSTANCE			mhInstance;	

	CWindowFrame(){}
	~CWindowFrame(){}

public:
	static CWindowFrame* Create(HINSTANCE _hinstance);
	static CWindowFrame* GetInstance();
	static void Destroy();

	static LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam);

	int Run(MSG _msg);
	void Initialize();
	void BuildWindow();
};
#endif
