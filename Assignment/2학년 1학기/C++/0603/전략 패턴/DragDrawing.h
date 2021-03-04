#ifndef _CDRAGDRAWING_H
#define _CDRAGDRAWING_H
#include "Global.h"
#include "Mouse.h"
#include "BackBit.h"

class CDragDrawing
{
protected:
	// 마우스 좌표
	static int mStartp_x;
	static int mStartp_y;

public:
	// 마우스 좌표 쓰기 함수
	static void SetStartp_x(int _x){ mStartp_x=_x;}
	static void SetStartp_y(int _y){ mStartp_y=_y;}

	virtual void DragDraw(CBackBit* _bit, CMouse* _mouse, int _drawmode) = 0;
};

// 자유선
class DragFreeLine : public CDragDrawing {
private:

public:
	DragFreeLine() : CDragDrawing() {

	}

	void DragDraw(CBackBit* _bit, CMouse* _mouse, int _drawmode) {
		if (_mouse->GetClick()) {

			HDC hdc = _bit->GetBitmapDC();
			MoveToEx(hdc, _mouse->GetOldX(), _mouse->GetOldy(), NULL);
			LineTo(hdc, _mouse->GetX(), _mouse->Gety());
			_bit->DeleteBitmapDC(hdc);
			InvalidateRect(_mouse->GetHWnd(), NULL, false);
		}
	}
};


// 직선
class DragLineDraw : public CDragDrawing {
private:

public:
	DragLineDraw() : CDragDrawing() {

	}

	void DragDraw(CBackBit* _bit, CMouse* _mouse, int _drawmode) {
		if (_mouse->GetClick())
		{
			if (_drawmode == REVERSE)
			{

				HDC hdc = _bit->GetBitmapDC();
				SetROP2(hdc, R2_NOT);
				MoveToEx(hdc, mStartp_x, mStartp_y, NULL);
				LineTo(hdc, _mouse->GetOldX(), _mouse->GetOldy());

				MoveToEx(hdc, mStartp_x, mStartp_y, NULL);
				LineTo(hdc, _mouse->GetX(), _mouse->Gety());
				_bit->DeleteBitmapDC(hdc);
				InvalidateRect(_mouse->GetHWnd(), NULL, false);
			}
			else
			{
				HDC hdc = _bit->GetBitmapDC();
				MoveToEx(hdc, mStartp_x, mStartp_y, NULL);
				LineTo(hdc, _mouse->GetX(), _mouse->Gety());
				_bit->DeleteBitmapDC(hdc);
				InvalidateRect(_mouse->GetHWnd(), NULL, false);

			}
		}
	}
};


// 원
class DragCircleDraw : public CDragDrawing {
private:

public:
	DragCircleDraw() : CDragDrawing() {

	}

	void DragDraw(CBackBit* _bit, CMouse* _mouse, int _drawmode) {
		if (_mouse->GetClick())
		{
			if (_drawmode == REVERSE)
			{
				HDC hdc = _bit->GetBitmapDC();
				HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)NULL_BRUSH);
				SetROP2(hdc, R2_NOT);
				Ellipse(hdc, mStartp_x, mStartp_y, _mouse->GetOldX(), _mouse->GetOldy());
				Ellipse(hdc, mStartp_x, mStartp_y, _mouse->GetX(), _mouse->Gety());
				DeleteObject(SelectObject(hdc, hOldBrush));
				_bit->DeleteBitmapDC(hdc);
				InvalidateRect(_mouse->GetHWnd(), NULL, false);
			}
			else
			{
				HDC hdc = _bit->GetBitmapDC();
				HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)NULL_BRUSH);
				Ellipse(hdc, mStartp_x, mStartp_y, _mouse->GetX(), _mouse->Gety());
				DeleteObject(SelectObject(hdc, hOldBrush));
				_bit->DeleteBitmapDC(hdc);
				InvalidateRect(_mouse->GetHWnd(), NULL, false);
			}
		}
	}
};

// 사각형
class DragRectangleDraw : public CDragDrawing {
private:

public:
	DragRectangleDraw() : CDragDrawing() {

	}

	void DragDraw(CBackBit* _bit, CMouse* _mouse, int _drawmode) {
		if (_mouse->GetClick())
		{
			if (_drawmode == REVERSE)
			{
				HDC hdc = _bit->GetBitmapDC();
				HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)NULL_BRUSH);
				SetROP2(hdc, R2_NOT);
				Rectangle(hdc, mStartp_x, mStartp_y, _mouse->GetOldX(), _mouse->GetOldy());
				Rectangle(hdc, mStartp_x, mStartp_y, _mouse->GetX(), _mouse->Gety());
				DeleteObject(SelectObject(hdc, hOldBrush));
				_bit->DeleteBitmapDC(hdc);
				InvalidateRect(_mouse->GetHWnd(), NULL, false);
			}
			else
			{
				HDC hdc = _bit->GetBitmapDC();
				HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)NULL_BRUSH);
				Rectangle(hdc, mStartp_x, mStartp_y, _mouse->GetX(), _mouse->Gety());
				DeleteObject(SelectObject(hdc, hOldBrush));
				_bit->DeleteBitmapDC(hdc);
				InvalidateRect(_mouse->GetHWnd(), NULL, false);
			}
		}
	}
};
#endif