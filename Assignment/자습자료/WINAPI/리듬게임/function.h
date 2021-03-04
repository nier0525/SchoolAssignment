#pragma once

#include <windows.h> // 윈도우 헤더파일
#include <iostream>
#include <mmsystem.h>	// 사운드
#include <stdlib.h>
// GDI 라이브러리
#include <gdiplus.h>
using namespace Gdiplus;
#include "resource.h"

#pragma comment(lib, "Gdiplus.lib")
#pragma comment(lib, "winmm.lib")

void DrawBitmap(HDC hdc, int x, int y, int brx, int bry, HBITMAP hBitmap, int r, int g, int b) {
	HDC MemDC;
	HBITMAP OldBitMap;
	int bx, by;
	BITMAP bitmap;

	MemDC = CreateCompatibleDC(hdc);
	OldBitMap = (HBITMAP)SelectObject(MemDC, hBitmap);

	GetObject(hBitmap, sizeof(BITMAP), &bitmap);
	bx = bitmap.bmWidth;
	by = bitmap.bmHeight;

	GdiTransparentBlt(hdc, x, y, brx, bry, MemDC, 0, 0, bx, by, RGB(r, g, b));

	SelectObject(MemDC, OldBitMap);
	DeleteDC(MemDC);
}

void DrawBitmap(HDC hdc, int x, int y, int brx, int bry, HBITMAP hBitmap, int bx, int by) {
	HDC MemDC;
	HBITMAP OldBitMap;
	BITMAP bitmap;

	MemDC = CreateCompatibleDC(hdc);
	OldBitMap = (HBITMAP)SelectObject(MemDC, hBitmap);

	GetObject(hBitmap, sizeof(BITMAP), &bitmap);

	BitBlt(hdc, x, y, brx, bry, MemDC, bx, by, SRCCOPY);

	SelectObject(MemDC, OldBitMap);
	DeleteDC(MemDC);
}

void DrawPNG(HDC hdc, Image& image, int x, int y, int w, int h) {
	Graphics graphic(hdc);
	graphic.DrawImage(&image, x, y, w, h);
}

Image* LoadPNG(HMODULE hModule, LPSTR ResourceName) {
	Image* image = nullptr;
	IStream* pStream = nullptr;
	PVOID pResourceData;
	DWORD imageSize;
	DWORD dwReadWrite;

	HRSRC hResource = FindResource(hModule, ResourceName, (LPCSTR)"PNG");

	if (CreateStreamOnHGlobal(nullptr, true, &pStream) == S_OK) {
		pResourceData = LockResource(LoadResource(hModule, hResource));
		imageSize = SizeofResource(hModule, hResource);
		dwReadWrite = 0;
		pStream->Write(pResourceData, imageSize, &dwReadWrite);
		image = Image::FromStream(pStream);
		pStream->Release();
	}
	return image;
}