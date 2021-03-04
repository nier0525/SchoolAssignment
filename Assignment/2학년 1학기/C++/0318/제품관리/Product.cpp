#include "Product.h"

cProduct::cProduct() {
	memset(mProductName, 0, sizeof(mProductName));
	memset(mID, 0, sizeof(mID));
	mCount = 0;
	mMoney = 0;
}

cProduct::~cProduct() {

}



int cProduct::GetCount() {
	return mCount;
}

int cProduct::GetMoney() {
	return mMoney;
}

const char* cProduct::GetName() {
	return mProductName;
}

const char* cProduct::GetID() {
	return mID;
}

void cProduct::MakeID() {
	
}

bool cProduct::SetCount(int _count) {
	if (_count < 0 || _count > SIZEMAX) {
		return false;
	}
	mCount = _count;
	return true;
}

bool cProduct::SetName(const char* _name) {
	if (strlen(_name) > NAMESIZE) {
		return false;
	}
	strcpy_s(mProductName, _name);
	return true;
}

bool cProduct::SetMoney(int _money) {
	if (_money < 0 || _money > MONEYMAX) {
		return false;
	}
	mMoney = _money;
	return true;
}

int cProduct::Add(int _count) {
	if (mCount + _count > SIZEMAX) { // ���� ���� + �԰� ������ �ִ� ������ �ʰ� �� ��
		int index = SIZEMAX - mCount; // ���� ���� index �� �ִ� ���� - ���� ������ �� ����
		mCount += index; // �׸��� ���� ���� ��� ������ ���� ( ��������� 10�� )
		return index; // ������ ���� ����
	}
	mCount += _count;
	return _count;
}

int cProduct::Delete(int _count) {
	if (mCount - _count <= 0) { // ���� ���� - �԰� ������ 0 ���� �۰ų� ���� ��
		int index = mCount; // ���� ���� index �� ���� ��� ������ ���� ���� ���� ����
		mCount = 0; // �׸��� ��� ���� �� 0 ���� �ʱ�ȭ
		return index; // index �� ����Ǿ� �ִ� ���� ����
	}
	mCount -= _count;
	return _count;
}