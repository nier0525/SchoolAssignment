#pragma once

#include "Global.h"

// Ű���� Ŭ����
// ��û �����ϰ� �����س��Ҵ�.
// �̱��� ����

class KeyBoard {
private:
	static KeyBoard* pthis;
	KeyBoard() {}
	~KeyBoard() {}

	HWND m_hWnd;	// �ڵ� ����
	WPARAM m_wParam;	// wParam �� ������ ����
	bool KeyDown;		// Ű���尡 ������ �� üũ

    char str[CHAR];		// ���ڿ� �Է� �� ���� ������ ����
	int len;			// ���ڿ� ����
public:
	static KeyBoard* GetInstance();
	static void Destory();

	void Init(HWND hWnd, WPARAM wParam);	// �ʱ�ȭ �۾�

	void SetKey(WPARAM wParam) { m_wParam = wParam; }	// wParam ���� ������
	void VK_move(int* x, int* y, int move);	// ��,��,��,�� �̵��� ���õ� ������ �Լ� ����
	void Init_Char();	// �Էµ� ���ڿ� �ʱ�ȭ

	void Down() { KeyDown = true; }	// Ű���� �ٿ�
	void Up() { KeyDown = false; }	// Ű���� ��
	bool State() { return KeyDown; }	// Ű���� ����

	// �б� ������
	WPARAM GetKeyCode() const { return m_wParam; }
	HWND GethWnd() { return m_hWnd; }
	const char* GetKeyChar();
};