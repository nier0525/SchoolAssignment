#pragma once

#include <windows.h>

class Mouse {
private:
	static Mouse* pthis;
	Mouse() {}
	~Mouse() {}

	POINT m_pos, m_oldpos;
	bool m_Lbutton, m_Rbutton;
public:
	static Mouse* GetInstance();
	static void Destory();

	void Init();

	void SetPos(LPARAM);
	void SetOldPos(LPARAM);

	void SetLbuttonDown();
	void SetLbuttonUp();

	void SetRbuttonDown();
	void SetRbuttonUp();

	bool GetLbuttonState();
	bool GetRbuttonState();

	int GetPosX();
	int GetPosY();

	int GetOldPosX();
	int GetOldPosY();
};