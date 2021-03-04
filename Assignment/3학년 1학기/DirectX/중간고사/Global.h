#pragma once

#include <windows.h>
#include <math.h>
#include <iostream>
using namespace std;

#define MAX_SIZE 1024
#define PI 3.141592

#define ToRadian(degree) ((degree) * (PI / 180.0f))
#define ToDegree(radian) ((radian) * (180.0f / PI))

#define MATRIX_ROTATEXYZ 0
#define MATRIX_ROTATEX 1
#define MATRIX_ROTATEY 2
#define MATRIX_ROTATEZ 3
#define MATRIX_SCALE 4
#define MATRIX_TRANSLATE 5
