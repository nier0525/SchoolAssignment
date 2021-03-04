#include "DragFreeLineDrawing.h"
CDragFreeLineDrawing* CDragFreeLineDrawing::mPthis = nullptr;

void CDragFreeLineDrawing::Create()
{
	if (!mPthis)
	{
		mPthis = new CDragFreeLineDrawing();
	}

}
CDragFreeLineDrawing* CDragFreeLineDrawing::GetInstance()
{
	return mPthis;
}
void CDragFreeLineDrawing::Destroy()
{
	if (mPthis)
	{
		delete mPthis;
		mPthis = nullptr;
	}
}

void CDragFreeLineDrawing::DragDrawing(CBackBit* _bit, int _drawmode)
{
	CMouse* mouse = CMouse::GetInstance();

	if (mouse->GetClick())
	{

		HDC hdc = _bit->GetBitmapDC();
		MoveToEx(hdc, mouse->GetOldX(), mouse->GetOldy(), NULL);
		m_Pen->SetPenColor(hdc, weight);
		LineTo(hdc, mouse->GetX(), mouse->Gety());
		m_Pen->DeletePen(hdc);
		_bit->DeleteBitmapDC(hdc);
		InvalidateRect(mouse->GetHWnd(), NULL, false);
	}

}