#pragma once

#include <Windows.h>
#include <stdio.h>
#include <math.h>

#define POINT_MAX 1024
#define PI 3.141592

struct Point2D {
	int x[POINT_MAX];
	int y[POINT_MAX];
};