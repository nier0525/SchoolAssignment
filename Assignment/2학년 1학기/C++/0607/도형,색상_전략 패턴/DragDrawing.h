#ifndef _CDRAGDRAWING_H
#define _CDRAGDRAWING_H
#include "Global.h"
#include "Mouse.h"
#include "BackBit.h"
#include "PenColor.h"
#include "MaskColor.h"

class CDragDrawing
{
protected:
	int mStartp_x;
	int mStartp_y;

	PenColor* m_Pen;
	MaskColor* m_Brush;

	int weight;
public:
	CDragDrawing(){
		m_Pen = RedPen::GetInstance();
		m_Brush = RedMask::GetInstance();
		weight = 1;
	}
	~CDragDrawing(){}

	int GetStartp_x() {return mStartp_x;}
	int GetStartp_y() {return mStartp_y;}

	void SetStartp_x(int _x){ mStartp_x=_x;}
	void SetStartp_y(int _y){ mStartp_y=_y;}

	virtual void DragDrawing(CBackBit* _bit, int _drawmode) = 0;

	void ChangePenColor(PenColor* _color) {
		m_Pen = _color;
	}

	void ChangeBrushColor(MaskColor* _color) {
		m_Brush = _color;
	}

	void SetWeight(int _weight) {
		weight = _weight;
	}
};

#endif