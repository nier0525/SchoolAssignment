#include "Manager.h"

int main() {
	cManager* program = new cManager(); // ��ü ���� �޸� �Ҵ�
	program->Run(); // Run() �Լ� ����

	delete program; // �޸� ����
	return 0;
}