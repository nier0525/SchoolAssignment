#include "DragRectangleDrawing.h"
CDragRectangleDrawing* CDragRectangleDrawing::mPthis = nullptr;

void CDragRectangleDrawing::Create()
{
	if (!mPthis)
	{
		mPthis = new CDragRectangleDrawing();
	}

}
CDragRectangleDrawing* CDragRectangleDrawing::GetInstance()
{
	return mPthis;
}
void CDragRectangleDrawing::Destroy()
{
	if (mPthis)
	{
		delete mPthis;
		mPthis = nullptr;
	}
}

void CDragRectangleDrawing::DragDrawing(CBackBit* _bit, int _drawmode)
{
	CMouse* mouse = CMouse::GetInstance();

	if (mouse->GetClick())
	{
		if (_drawmode == REVERSE)
		{
			HDC hdc = _bit->GetBitmapDC();

			m_Pen->SetPenColor(hdc, weight);
			m_Brush->SetMaskColor(hdc);

			SetROP2(hdc, R2_NOTXORPEN);

			Rectangle(hdc, mStartp_x, mStartp_y, mouse->GetOldX(), mouse->GetOldy());
			Rectangle(hdc, mStartp_x, mStartp_y, mouse->GetX(), mouse->Gety());
			m_Pen->DeletePen(hdc);
			m_Brush->DeleteBrush(hdc);

			_bit->DeleteBitmapDC(hdc);
			InvalidateRect(mouse->GetHWnd(), NULL, false);
		}
		else
		{
			HDC hdc = _bit->GetBitmapDC();

			m_Pen->SetPenColor(hdc, weight);
			m_Brush->SetMaskColor(hdc);
			Rectangle(hdc, mStartp_x, mStartp_y, mouse->GetX(), mouse->Gety());
			m_Pen->DeletePen(hdc);
			m_Brush->DeleteBrush(hdc);
		
			_bit->DeleteBitmapDC(hdc);
			InvalidateRect(mouse->GetHWnd(), NULL, false);
		}
	}

}