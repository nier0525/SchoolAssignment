#include "Menu.h"

void CMenu::MenuInitialize(int _id, HWND _hwnd)
{
	mHmainWnd = _hwnd;
	mHmenu = GetMenu(mHmainWnd);
	mMenuId=_id;
	mDrawing = nullptr;
}
void CMenu::ProcessCommandMeg(WPARAM _wparam)
{
	
	switch(LOWORD(_wparam))
	{
	// ----------------------------------------------------
	case LINE_RED:
		mDrawing = CDragLineDrawing::GetInstance();		
		mDrawing->ChangePenColor(RedPen::GetInstance());
		break;

	case LINE_BLUE:
		mDrawing = CDragLineDrawing::GetInstance();
		mDrawing->ChangePenColor(BluePen::GetInstance());
		break;

	case LINE_GREEN:
		mDrawing = CDragLineDrawing::GetInstance();
		mDrawing->ChangePenColor(GreenPen::GetInstance());
		break;

	case LINE_1:
		mDrawing = CDragLineDrawing::GetInstance();
		mDrawing->SetWeight(1);
		break;

	case LINE_3:
		mDrawing = CDragLineDrawing::GetInstance();
		mDrawing->SetWeight(3);
		break;

	case LINE_5:
		mDrawing = CDragLineDrawing::GetInstance();
		mDrawing->SetWeight(5);
		break;

	// ----------------------------------------------------
	case LINE_FREE_RED:
		mDrawing = CDragFreeLineDrawing::GetInstance();
		mDrawing->ChangePenColor(RedPen::GetInstance());
		break;

	case LINE_FREE_BLUE:
		mDrawing = CDragFreeLineDrawing::GetInstance();
		mDrawing->ChangePenColor(BluePen::GetInstance());
		break;

	case LINE_FREE_GREEN:
		mDrawing = CDragFreeLineDrawing::GetInstance();
		mDrawing->ChangePenColor(GreenPen::GetInstance());
		break;

	case LINE_FREE_1:
		mDrawing = CDragFreeLineDrawing::GetInstance();
		mDrawing->SetWeight(1);
		break;

	case LINE_FREE_3:
		mDrawing = CDragFreeLineDrawing::GetInstance();
		mDrawing->SetWeight(3);
		break;

	case LINE_FREE_5:
		mDrawing = CDragFreeLineDrawing::GetInstance();
		mDrawing->SetWeight(5);
		break;

	// ----------------------------------------------------
	case ONE_LINE_RED:
		mDrawing = CDragCircleDrawing::GetInstance();
		mDrawing->ChangePenColor(RedPen::GetInstance());
		break;

	case ONE_LINE_BLUE:
		mDrawing = CDragCircleDrawing::GetInstance();
		mDrawing->ChangePenColor(BluePen::GetInstance());
		break;

	case ONE_LINE_GREEN:
		mDrawing = CDragCircleDrawing::GetInstance();
		mDrawing->ChangePenColor(GreenPen::GetInstance());
		break;

	case ONE_LINE_1:
		mDrawing = CDragCircleDrawing::GetInstance();
		mDrawing->SetWeight(1);
		break;

	case ONE_LINE_3:
		mDrawing = CDragCircleDrawing::GetInstance();
		mDrawing->SetWeight(3);
		break;

	case ONE_LINE_5:
		mDrawing = CDragCircleDrawing::GetInstance();
		mDrawing->SetWeight(5);
		break;

	case ONE_MASK_RED:
		mDrawing = CDragCircleDrawing::GetInstance();
		mDrawing->ChangeBrushColor(RedMask::GetInstance());
		break;

	case ONE_MASK_BLUE:
		mDrawing = CDragCircleDrawing::GetInstance();
		mDrawing->ChangeBrushColor(BlueMask::GetInstance());
		break;

	case ONE_MASK_GREEN:
		mDrawing = CDragCircleDrawing::GetInstance();
		mDrawing->ChangeBrushColor(GreenMask::GetInstance());
		break;

	// ----------------------------------------------------
	case RECT_LINE_RED:
		mDrawing = CDragRectangleDrawing::GetInstance();
		mDrawing->ChangePenColor(RedPen::GetInstance());
		break;

	case RECT_LINE_BLUE:
		mDrawing = CDragRectangleDrawing::GetInstance();
		mDrawing->ChangePenColor(BluePen::GetInstance());
		break;

	case RECT_LINE_GREEN:
		mDrawing = CDragRectangleDrawing::GetInstance();
		mDrawing->ChangePenColor(GreenPen::GetInstance());
		break;

	case RECT_LINE_1:
		mDrawing = CDragRectangleDrawing::GetInstance();
		mDrawing->SetWeight(1);
		break;

	case RECT_LINE_3:
		mDrawing = CDragRectangleDrawing::GetInstance();
		mDrawing->SetWeight(3);
		break;

	case RECT_LINE_5:
		mDrawing = CDragRectangleDrawing::GetInstance();
		mDrawing->SetWeight(5);
		break;

	case RECT_MASK_RED:
		mDrawing = CDragRectangleDrawing::GetInstance();
		mDrawing->ChangeBrushColor(RedMask::GetInstance());
		break;

	case RECT_MASK_BLUE:
		mDrawing = CDragRectangleDrawing::GetInstance();
		mDrawing->ChangeBrushColor(BlueMask::GetInstance());
		break;

	case RECT_MASK_GREEN:
		mDrawing = CDragRectangleDrawing::GetInstance();
		mDrawing->ChangeBrushColor(GreenMask::GetInstance());
		break;

	// ----------------------------------------------------
	case ID_QUIT:
		DestroyWindow(mHmainWnd);
		break;
	}

	//switch (mSelectMenu) {

	//case LINE:
	//	mDrawing = CDragLineDrawing::GetInstance();
	//	break;

	//case FREELINE:
	//	mDrawing = CDragFreeLineDrawing::GetInstance();
	//	break;

	//case CIRCLE:
	//	mDrawing = CDragCircleDrawing::GetInstance();
	//	break;

	//case RECTANGLE:
	//	mDrawing = CDragRectangleDrawing::GetInstance();
	//	break;
	//}
}

void CMenu::SetMenuHandle(HMENU _hmenu)
{
	mHmenu=_hmenu;
}

void CMenu::SetSelectMenu(int _menu)
{
	mSelectMenu=_menu;
}