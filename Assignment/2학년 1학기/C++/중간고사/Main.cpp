#include "GameManager.h"

int main() {
	GameManager* System = new GameManager(); // GameManager ���� �Ҵ�
	System->Run(); // Run �Լ� ����
	delete System; // Run �� ������ �޸� ��ȯ
}