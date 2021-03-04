#pragma once

#include "Mouse.h"
#include "Collision.h"
#include "BackBit.h"
#include "KeyBoard.h"

#include "LoginManager.h"
#include "CharacterManager.h"

// ���� �Ŵ��� Ŭ����
// ��� �޴����� �θ� Ŭ�����̰�, �޴��� �׸��� �Լ��� ���Ǵ� �ٿ� ĳ������ ���� �ڽĿ��� �ñ��.

class GameManager {
protected:
	CMouse* mouse;	// ���콺 ��ü ����
	Collision* col;	// �浹 ���� ��ü ����
public:
	GameManager() {
		mouse = CMouse::GetInstance();
		col = Collision::GetInstance();
	}
	~GameManager() {}

	virtual void DrawMenu(CBackBit* _bit) = 0;
};