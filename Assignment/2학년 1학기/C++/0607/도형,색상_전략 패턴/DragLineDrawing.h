#ifndef _CDRAGLINE_H
#define _CDRAGLINE_H

#include "Global.h"
#include "DragDrawing.h"

class CDragLineDrawing : public CDragDrawing
{
	static CDragLineDrawing*	mPthis;

	CDragLineDrawing() {}
	~CDragLineDrawing() {}

public:
	static void Create();
	static CDragLineDrawing* GetInstance();
	static void Destroy();

	void DragDrawing(CBackBit* _bit, int _drawmode);
};

#endif