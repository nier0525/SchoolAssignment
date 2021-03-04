#include "DragCircleDrawing.h"
CDragCircleDrawing* CDragCircleDrawing::mPthis = nullptr;

void CDragCircleDrawing::Create()
{
	if (!mPthis)
	{
		mPthis = new CDragCircleDrawing();
	}

}
CDragCircleDrawing* CDragCircleDrawing::GetInstance()
{
	return mPthis;
}
void CDragCircleDrawing::Destroy()
{
	if (mPthis)
	{
		delete mPthis;
		mPthis = nullptr;
	}
}

void CDragCircleDrawing::DragDrawing(CBackBit* _bit, int _drawmode)
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

			Ellipse(hdc, mStartp_x, mStartp_y, mouse->GetOldX(), mouse->GetOldy());
			Ellipse(hdc, mStartp_x, mStartp_y, mouse->GetX(), mouse->Gety());
		
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
			Ellipse(hdc, mStartp_x, mStartp_y, mouse->GetX(), mouse->Gety());
			m_Pen->DeletePen(hdc);
			m_Brush->DeleteBrush(hdc);
			_bit->DeleteBitmapDC(hdc);
			InvalidateRect(mouse->GetHWnd(), NULL, false);
		}
	}

}