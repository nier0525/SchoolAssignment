#ifndef CDATA_H
#define CDATA_H
#include "Base.h"

class cFruit {
private:
	int mCount;
	int mMoney; // ��� ����
public:
	cFruit(); // ������
	~cFruit(); // �Ҹ���

	void SetCount(int);
	void SetMoney(int);

	int GetCount();
	int GetMoney();
	// �Լ�
};

#endif