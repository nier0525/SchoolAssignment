#include "KeyBoard.h"

KeyBoard* KeyBoard::pthis = nullptr;

KeyBoard* KeyBoard::GetInstance() {
	if (!pthis) {
		pthis = new KeyBoard();
	}
	return pthis;
}

void KeyBoard::Destory() {
	if (pthis) {
		delete pthis;
	}
}

// �ʱ�ȭ �۾�
void KeyBoard::Init(HWND hWnd, WPARAM wParam) {
	m_hWnd = hWnd;
	m_wParam = wParam;
	KeyDown = false;

	memset(str, NULL, sizeof(str));
	len = NULL;
}

// �Է��� ���ڿ� �ʱ�ȭ
void KeyBoard::Init_Char() {
	memset(str, NULL, sizeof(str));
	len = NULL;
}

// ����� ��ǥ�� ����Ű�� ���� �̵������ִ� ������ �Լ�
void KeyBoard::VK_move(int* x, int* y, int move) {
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		*x -= move;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		*x += move;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		*y += move;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		*y -= move;
	}
}

// Ű���忡�� ���ڿ��� �Է� �޾� ��ȯ���ش�.
const char* KeyBoard::GetKeyChar() {
	len = strlen(str);	// ���ڿ��� ���̸� �����Ѵ�.
	if (len <= 20) {	// ���ڿ��� �ִ� 20 ����
		str[len] = (char)m_wParam;	// Ű���� �Է��� char �� ����ȯ ��Ų �� str ���ڿ��� �����Ѵ�.
		str[len + 1] = NULL;	// �Էµ� ���ڿ� ���� ���ڿ��� nullptr
	}

	if (m_wParam == VK_BACK && len != NULL) {	// ���ڿ��� �� ���¿��� ��Ű�� ���� �� �ƹ��͵� ���Ѵ�.
		str[len - 1] = NULL;
	}
	if (m_wParam == VK_BACK && len == NULL) {	// ��Ű�� ������ �Էµ� ���� �� �ϳ��� �����.
		str[len] = NULL;
	}

	return str; // ���ڿ� ��ȯ
}