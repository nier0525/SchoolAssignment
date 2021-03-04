#pragma once

#include "Global.h"

// ���콺�� ������ �浹�� ���� Ŭ����
// �̱��� ����

class Collision {
private:
	static Collision* pthis;
	Collision() {}
	~Collision() {}
public:
	static Collision* GetInstance();
	static void Destory();

	// �Ÿ� ����
	double Lengthpts(int x1, int y1, int x2, int y2);

	// ���� ���� �ε��� ��
	bool CollisionEnterinCircle(int x, int y, int mx, int my, int size);
	// �簢���� ��(���콺)�� �ε��� ��
	bool CollisionEnterinPoint(int x, int y, int mx, int my, int x_size, int y_size);
};