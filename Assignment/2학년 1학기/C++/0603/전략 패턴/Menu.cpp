#include "Menu.h"

void CMenu::MenuInitialize(int _id)
{
	//mHmenu=;	
	mMenuId=_id;
	NowDragDraw = new DragFreeLine();
}
void CMenu::PorcessCommandMeg(WPARAM _wparam)
{
	
	switch(LOWORD(_wparam))
	{
	case ID_FREELINE:
		ChangeDragDraw(new DragFreeLine);
		break;
	case ID_LINE:
		ChangeDragDraw(new DragLineDraw);
		break;
	case ID_CIRCLE:
		ChangeDragDraw(new DragCircleDraw);
		break;
	case ID_RECTANGLE:
		ChangeDragDraw(new DragRectangleDraw);
		break;
	case ID_EXIT:
		PostQuitMessage(0);
		break;
	}
}

void CMenu::SetMenuHandle(HMENU _hmenu)
{
	mHmenu=_hmenu;
}

void CMenu::SetSelectMenu(int _menu)
{
	mSelectMenu=_menu;
}