#ifndef CMANAGER_H
#define CMANAGER_H
#include "Base.h"
#include "Data.h"

class cManager {
private:
	cFruit*		mFruit[ASIZE]; // ��ü ��� ����
public:
	cManager(); // ������
	~cManager(); // �Ҹ���

	void Input(); 
	void Output();
	void Show();
	void ShowMenu();
	
	void Run();
	// �Լ�
};

#endif
