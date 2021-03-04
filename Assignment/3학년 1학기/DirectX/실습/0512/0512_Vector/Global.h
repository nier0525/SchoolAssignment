#pragma once
#include <windows.h>
#include <math.h>
#include <iostream>
using namespace std;

#define MAX_SIZE 1024
#define PI 3.141592

struct Point2D {
	float x, y;

	// »ý¼ºÀÚ
	Point2D() {};
	Point2D(float _x, float _y) {
		x = _x;
		y = _y;
	}
};