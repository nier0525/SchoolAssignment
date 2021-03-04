#include "MyMath.h"

int LinearEquation(float x1, float x2, float y1, float y2, float* a, float* b) {
	if (abs((int)(x2 - x1)) > abs((int)(y2 - y1))) {
		*a = (y2 - y1) / (x2 - x1);
		*b = y1 - (*a * x1);
		
		return 1;
	}
	else if ((y2 - y1) != 0) {
		*a = (x2 - x1) / (y2 - y1);
		*b = x1 - (*a * y1);

		return 0;
	}
}

void SimpleLine(HDC hdc, float x1, float y1, float x2, float y2)
{
	float m, n = 0;
	
	if (LinearEquation(x1, x2, y1, y2, &m, &n))
	{		
		float dx = (x2 > x1) ? 1 : -1;

		while (x1 != x2) {
			SetPixel(hdc, x1, y1, RGB(0, 0, 0));

			x1 += dx;
			y1 = (int)(m*(float)x1 + n);
		}
	}
	else {
		float dy = ((y2 - y1) < 0) ? -1 : 1;

		while (y1 != y2) {
			SetPixel(hdc, x1, y1, RGB(0, 0, 0));

			y1 += dy;
			x1 = (int)(m*(float)y1 + n);
		}
	}
	SetPixel(hdc, x1, y1, RGB(0, 0, 0));
}

int SimpleLine(Point2D point[], float x2, float y2)
{
	float m, n = 0;
	float x, y;
	int index = 0;

	x = point[index].x = 0;
	y = point[index].y = 0;
	index++;

	if (LinearEquation(point[index].x, x2, point[index].y, y2, &m, &n))
	{
		float dx = (x2 > point[index].x) ? 1 : -1;

		while (1) {
			if (dx > 0 && x >= x2) break;
			else if (dx < 0 && x <= x2) break;

			x += dx;
			point[index].x = x;
			point[index].y = (int)(m*(float)point[index].x + n);
			index++;
		}
	}
	else {
		float dy = ((y2 - point[index].y) < 0) ? -1 : 1;

		while (1) {
			if (dy > 0 && y >= y2) break;
			else if (dy < 0 && y <= y2) break;

			y += dy;
			point[index].y = y;
			point[index].x = (int)(m*(float)point[index].y + n);
			index++;
		}
	}

	return index;
}

void Field(HDC hdc, Point2D Start, Point2D End)
{
	SimpleLine(hdc, Start.x, Start.y, End.x, Start.y);
	SimpleLine(hdc, Start.x, Start.y, Start.x, End.y);
	SimpleLine(hdc, End.x, End.y, End.x, Start.y);
	SimpleLine(hdc, End.x, End.y, Start.x, End.y);
}

void Axis(HDC hdc, Point2D Start, Point2D End) 
{
	SimpleLine(hdc, (Start.x + End.x) / 2, Start.y, (End.x + Start.x) / 2, End.y);
	SimpleLine(hdc, Start.x, (Start.y + End.y) / 2, End.x, (End.y + Start.y) / 2);
}

float Viewport_Convert_Y(float y) {
	return y *= -1;
}

void Viewport_Convert_Y(float* y) {
	*y *= -1;
}

void Viewport_Convert_XY(Point2D Min, Point2D Max, float mx, float my, float* x, float* y) {
	*x = (float)((mx - Min.x) - (Max.x - Min.x) / 2.0) * (float)(1.0 / ((Max.x - Min.x) / 2.0));
	*y = -(float)((my - Min.y) - (Max.y - Min.y) / 2.0) * (float)(1.0 / ((Max.y - Min.y) / 2.0));
}