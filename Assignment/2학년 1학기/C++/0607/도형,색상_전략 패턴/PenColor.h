#pragma once

#include "Global.h"
#include "BackBit.h"

class PenColor {
protected:
	HPEN hPen, oldPen;
public:
	PenColor() {
		hPen = nullptr;
		oldPen = nullptr;
	}
	~PenColor() {}

	virtual void SetPenColor(HDC hdc, int weight) = 0;

	void DeletePen(HDC hdc) {
		SelectObject(hdc, oldPen);
		DeleteObject(hPen);
	}
};

class RedPen : public PenColor {
private:
	static RedPen* pthis;
	RedPen() {}
	~RedPen() {}

public:
	static RedPen* GetInstance();
	static void Destory();

	void SetPenColor(HDC hdc, int weight) {
		hPen = CreatePen(PS_SOLID, weight, RGB(255, 0, 0));
		oldPen = (HPEN)SelectObject(hdc, hPen);
	}
};

class GreenPen : public PenColor {
private:
	static GreenPen* pthis;
	GreenPen() {}
	~GreenPen() {}

public:
	static GreenPen* GetInstance();
	static void Destory();

	void SetPenColor(HDC hdc, int weight) {
		hPen = CreatePen(PS_SOLID, weight, RGB(0, 255, 0));
		oldPen = (HPEN)SelectObject(hdc, hPen);
	}
};



class BluePen : public PenColor {
private:
	static BluePen* pthis;
	BluePen() {}
	~BluePen() {}

public:
	static BluePen* GetInstance();
	static void Destory();

	void SetPenColor(HDC hdc, int weight) {
		hPen = CreatePen(PS_SOLID, weight, RGB(0, 0, 255));
		oldPen = (HPEN)SelectObject(hdc, hPen);
	}
};
