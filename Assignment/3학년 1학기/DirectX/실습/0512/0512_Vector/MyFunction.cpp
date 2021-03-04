#include "MyFunction.h"

bool CollisionEnterPoint(int x, int y, int mx, int my, int x_size, int y_size) {
	if ((x + x_size >= mx && mx >= x - x_size) && (y + y_size >= my && my >= y - y_size)) {
		return true;
	}
	else {
		return false;
	}
}
