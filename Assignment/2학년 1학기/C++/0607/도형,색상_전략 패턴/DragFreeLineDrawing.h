#ifndef _CDRAGFREELINE_H
#define _CDRAGFREELINE_H

#include "Global.h"
#include "DragDrawing.h"

class CDragFreeLineDrawing:public CDragDrawing
{
	static CDragFreeLineDrawing*	mPthis;
	
	CDragFreeLineDrawing(){}
	~CDragFreeLineDrawing(){}

public:
	static void Create();
	static CDragFreeLineDrawing* GetInstance();
	static void Destroy();

	void DragDrawing(CBackBit* _bit, int _drawmode);

};
#endif