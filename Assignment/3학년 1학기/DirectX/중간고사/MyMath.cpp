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

int SimpleLine(Vector3 point[], float x1, float y1, float x2, float y2)
{
	float m, n = 0;
	float x, y;
	int index = 0;

	x = point[index].x = x1;
	y = point[index].y = y1;
	index++;

	if (LinearEquation(x1, x2, y1, y2, &m, &n))
	{
		float dx = (x2 > x1) ? 1 : -1;

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
		float dy = ((y2 - y1) < 0) ? -1 : 1;

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

int SimpleCircle(Vector3 point[], float x, float y, float radian)
{
	int start_x, start_y;
	int finder;
	int index = 0;

	start_x = 0;
	start_y = radian;
	finder = 3 - 2 * radian;

	while (start_x <= start_y)
	{
		point[index++] = Vector3(x + start_x, y + start_y, 0);
		point[index++] = Vector3(x + start_x, y - start_y, 0);
		point[index++] = Vector3(x - start_x, y + start_y, 0);
		point[index++] = Vector3(x - start_x, y - start_y, 0);
	
		point[index++] = Vector3(x + start_y, y + start_x, 0);
		point[index++] = Vector3(x + start_y, y - start_x, 0);
		point[index++] = Vector3(x - start_y, y + start_x, 0);
		point[index++] = Vector3(x - start_y, y - start_x, 0);

		if (finder < 0)
		{
			finder += 4 * start_x + 6;
			start_x++;
		}
		else
		{
			finder += 4 * (start_x - start_y) + 10;
			start_x++;
			start_y--;
		}
	}

	return index;
}

float Viewport_Convert_Y(float y) {
	return y *= -1;
}

void Viewport_Convert_Y(float* y) {
	*y *= -1;
}
