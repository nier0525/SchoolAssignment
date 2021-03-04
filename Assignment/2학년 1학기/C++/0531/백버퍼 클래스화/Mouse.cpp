#include "Mouse.h"

Mouse* Mouse::pthis = nullptr;

Mouse* Mouse::GetInstance() {
	if (!pthis) {
		pthis = new Mouse();
	}
	return pthis;
}

void Mouse::Destory() {
	if (pthis) {
		delete pthis;
	}
}

void Mouse::Init() {
	m_pos = { 0,0 };
	m_oldpos = { 0,0 };
	m_Lbutton = false;
	m_Rbutton = false;
}

void Mouse::SetPos(LPARAM lParam) {
	m_oldpos = m_pos;

	m_pos.x = LOWORD(lParam);
	m_pos.y = HIWORD(lParam);
}

void Mouse::SetOldPos(LPARAM lParam) {
	m_oldpos.x = LOWORD(lParam);
	m_oldpos.y = HIWORD(lParam);
}

void Mouse::SetLbuttonDown() {
	m_Lbutton = true;
}

void Mouse::SetRbuttonDown() {
	m_Rbutton = true;
}

void Mouse::SetLbuttonUp() {
	m_Lbutton = false;
}

void Mouse::SetRbuttonUp() {
	m_Rbutton = false;
}

bool Mouse::GetLbuttonState() {
	return m_Lbutton;
}

bool Mouse::GetRbuttonState() {
	return m_Rbutton;
}

int Mouse::GetPosX() {
	return m_pos.x;
}

int Mouse::GetPosY() {
	return m_pos.y;
}

int Mouse::GetOldPosX() {
	return m_oldpos.x;
}

int Mouse::GetOldPosY() {
	return m_oldpos.y;
}