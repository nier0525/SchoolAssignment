#include "Data.h"

cFruit::cFruit() {
	mMoney = 0;	mCount = 0; // ��� ���� �� �ʱ�ȭ
}

cFruit::~cFruit() {}

void cFruit::SetCount(int _Count) {
	mCount = _Count; // �ܺ��� int ���� ����
}

void cFruit::SetMoney(int _Money) {
	mMoney = _Money; // �ܺ��� int ���� ����
}

int cFruit::GetCount() {
	return mCount; // ���� �� ����
}

int cFruit::GetMoney() {
	return mMoney; // ���� �� ����
}