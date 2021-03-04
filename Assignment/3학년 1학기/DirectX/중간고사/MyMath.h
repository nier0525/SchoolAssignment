#pragma once
#include "Global.h"
#include "Vector3.h"

#define MAX_RANGE 1
#define MIN_RANGE -1

int LinearEquation(float, float, float, float, float*, float*);
int SimpleLine(Vector3[], float, float, float, float);
int SimpleCircle(Vector3[], float, float, float);

float Viewport_Convert_Y(float);
void Viewport_Convert_Y(float*);
