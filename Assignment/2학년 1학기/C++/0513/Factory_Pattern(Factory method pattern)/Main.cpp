#include "Global.h"
#include "AbstractGimBabStore.h"

int main(void)
{
	AbstractGimBabStore* store = new AbstractGimBabStore();
	cout << "1. ġ������" << endl << "2. ��ä�����" << endl << "���� : ";
	int sel = 0;
	cin >> sel;
	CGimBab* gimbab = store->CreateGimBabStore(sel)->OrderGimBab();
	delete store;
	delete gimbab;
	return 0;
}