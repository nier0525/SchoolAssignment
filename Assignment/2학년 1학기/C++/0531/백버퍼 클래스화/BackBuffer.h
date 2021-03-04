#pragma once

#include <windows.h>

class BackBuffer {
private:
	HDC m_backDC;
	HBITMAP m_back, m_oldback;
	RECT rt;
public:
	BackBuffer();
	~BackBuffer();

	void CreateBackBuffer(HWND, HDC, RECT, RECT);
	void ReleaseBackBuffer();
};