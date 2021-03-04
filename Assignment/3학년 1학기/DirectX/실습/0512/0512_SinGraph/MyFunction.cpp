#include "MyFunction.h"

// ������ �������� ��ȯ
double Getradian(double degree)
{
	return PI * degree / 180.0;
}

// sin �׷��� �Լ�
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