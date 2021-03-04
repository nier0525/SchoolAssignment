#pragma once
#include <windows.h>
#include <stdio.h>

#include "ListNode.h"

#define MAX_RANGE 1
#define MIN_RANGE -1

struct Point2D;

int LinearEquation(float x1, float x2, float y1, float y2, float* a, float* b);
void SimpleLine(HDC hdc, float x1, float y1, float x2, float y2);

void Field(HDC hdc, Point2D Start, Point2D End);
void CreateCircle(ListNode<Point2D*>*, Point2D*);
void Axis(HDC, Point2D, Point2D);

void Viewport_Convert_XY(Point2D Min, Point2D Max, float mx, float my, float* x, float* y);

struct Point2D {
	float x, y;

	// »ý¼ºÀÚ
	Point2D() {};
	Point2D(float _x, float _y) {
		x = _x;
		y = _y;
	}
	Point2D(float _x, float _y, Point2D Min, Point2D Max, float mx, float my) {
		x = _x;
		y = _y;

		if (mx > MAX_RANGE) x = Max.x;
		if (my > MAX_RANGE) y = Min.y;
		if (mx < MIN_RANGE) x = Min.x;
		if (my < MIN_RANGE) y = Max.y;

	}
};
