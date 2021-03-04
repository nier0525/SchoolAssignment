#include "Collision.h"

Collision* Collision::pthis = nullptr;

Collision* Collision::GetInstance() {
	if (!pthis) {
		pthis = new Collision();
	}
	return pthis;
}

void Collision::Destory() {
	if (pthis) {
		delete pthis;
	}
}

// �浹ü�� �浹ü�� �Ÿ��� ����ϴ� �Լ�
double Collision::Lengthpts(int x1, int y1, int x2, int y2) {
	return (sqrt((float)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1))));
}

// ���� ���� �浹�� ���� ������ ���� �Լ�
bool Collision::CollisionEnterinCircle(int x, int y, int mx, int my, int size) {
	if (Collision::GetInstance()->Lengthpts(x, y, mx, my) < size) { return true; }
	else { return false; }
}

// �簢���� ��(���콺) �� �浹�� ���� ������ ���� �Լ�
bool Collision::CollisionEnterinPoint(int x, int y, int mx, int my, int x_size, int y_size) {
	if ((x + x_size >= mx && mx >= x - x_size) && (y + y_size >= my && my >= y - y_size)) { return true; }
	else { return false; }
}