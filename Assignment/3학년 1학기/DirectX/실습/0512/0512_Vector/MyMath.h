#pragma once
#include "Global.h"
#include "ListNode.h"

#define MAX_RANGE 1
#define MIN_RANGE -1

int LinearEquation(float, float, float, float, float* , float* );
void SimpleLine(HDC, float, float, float, float);
int SimpleLine(Point2D[], float, float);

void Field(HDC, Point2D, Point2D);
void Axis(HDC, Point2D, Point2D);

float Viewport_Convert_Y(float);
void Viewport_Convert_Y(float*);

void Viewport_Convert_XY(Point2D, Point2D, float, float, float*, float*);
