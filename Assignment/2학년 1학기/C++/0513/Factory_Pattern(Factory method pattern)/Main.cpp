#include "Global.h"
#include "AbstractGimBabStore.h"

int main(void)
{
	AbstractGimBabStore* store = new AbstractGimBabStore();
	cout << "1. Ä¡Áî±è¹äÁ¡" << endl << "2. ¾ßÃ¤±è¹äÁ¡" << endl << "¼±ÅÃ : ";
	int sel = 0;
	cin >> sel;
	CGimBab* gimbab = store->CreateGimBabStore(sel)->OrderGimBab();
	delete store;
	delete gimbab;
	return 0;
}