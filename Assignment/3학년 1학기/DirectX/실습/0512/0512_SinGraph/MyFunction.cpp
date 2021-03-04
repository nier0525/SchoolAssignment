#include "MyFunction.h"

// 각도를 라디안으로 변환
double Getradian(double degree)
{
	return PI * degree / 180.0;
}

// sin 그래프 함수
void Sin_Graph(HDC hdc, int x[], int y[])
{
	int degree;
	double radian;

	for (degree = 0; degree <= POINT_MAX; degree++) {
		radian = sin(Getradian(degree));
		x[degree] = degree;
		y[degree] = 100 + (int)(100.0 * radian);
	}
}