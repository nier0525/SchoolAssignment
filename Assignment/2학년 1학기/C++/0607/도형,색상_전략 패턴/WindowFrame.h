#ifndef _CWINDOWFRAME_H
#define _CWINDOWFRAME_H
#include "Global.h"
#include "Menu.h"
#include "DragCircleDrawing.h"
#include "DragFreeLineDrawing.h"
#include "DragLineDrawing.h"
#include "DragRectangleDrawing.h"


class CWindowFrame
{
	static CWindowFrame* mPthis;

	HWND				mHWnd;	
	HINSTANCE			mHinstance;	

	CBackBit*			mBackbit;
	CMenu*				mMenu;	

	CWindowFrame();
	~CWindowFrame(){}

public:
	static CWindowFrame* Create(HINSTANCE _hinstance);
	static CWindowFrame* GetInstance();
	static void Destroy();

	HWND GetHWnd()const{ return mHWnd; }
	HINSTANCE GetHInstance() const{ return mHinstance; }

	static LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam);	

	void Run(MSG _msg);
	void Initialize();
	void BuildWindow();
	
};
#endif
