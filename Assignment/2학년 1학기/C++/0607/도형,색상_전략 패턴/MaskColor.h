#pragma once

#include "Global.h"

class MaskColor {
protected:
	HBRUSH hBrush, oldBrush;
public:
	MaskColor() {
		hBrush = nullptr;
		oldBrush = nullptr;
	}
	~MaskColor() {}

	virtual void SetMaskColor(HDC hdc) = 0;
	void DeleteBrush(HDC hdc) {
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);
	}
};

// --------------------------------------------

class RedMask : public MaskColor {
private:
	static RedMask* pthis;
	RedMask() {}
	~RedMask() {}

public:
	static RedMask* GetInstance();
	static void Destory();

	void SetMaskColor(HDC hdc) {
		hBrush = CreateSolidBrush(RGB(255, 0, 0));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	}
};


// --------------------------------------------

class BlueMask : public MaskColor {
private:
	static BlueMask* pthis;
	BlueMask() {}
	~BlueMask() {}

public:
	static BlueMask* GetInstance();
	static void Destory();

	void SetMaskColor(HDC hdc) {
		hBrush = CreateSolidBrush(RGB(0, 0, 255));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	}
};


// --------------------------------------------

class GreenMask : public MaskColor {
private:
	static GreenMask* pthis;
	GreenMask() {}
	~GreenMask() {}

public:
	static GreenMask* GetInstance();
	static void Destory();

	void SetMaskColor(HDC hdc) {
		hBrush = CreateSolidBrush(RGB(0, 255, 0));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	}
 };
