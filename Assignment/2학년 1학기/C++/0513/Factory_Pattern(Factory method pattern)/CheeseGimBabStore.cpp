#include "CheeseGimBabStore.h"

CCheeseGimBabStore::CCheeseGimBabStore()
{
	memset(mCheeseGimbabMenu, 0, sizeof(mCheeseGimbabMenu));
	mCount = 0;

	mCheeseGimbabMenu[mCount++] = new CGimBabStore::Menu("¸ðÂ¥·¼¶ó Ä¡Áî±è¹ä", 3000);
	mCheeseGimbabMenu[mCount++] = new CGimBabStore::Menu("Å©¸² Ä¡Áî ±è¹ä", 4000);
	mCheeseGimbabMenu[mCount++] = new CGimBabStore::Menu("Ã¼´Ù Ä¡Áî ±è¹ä", 3500);

}
CCheeseGimBabStore::~CCheeseGimBabStore()
{
	for (int i = 0; i < mCount; i++)
	{
		if (mCheeseGimbabMenu[i] != nullptr)
		{
			delete mCheeseGimbabMenu[i];
		}
	}

	cout << "Ä¡Áî ±è¹ä°¡°Ô°¡ ¹®À» ´Ý½À´Ï´Ù. " << endl;
}

CGimBab* CCheeseGimBabStore::CreateGimBab(const char* _name, int _price)
{
	CGimBab* gimbab = new CCheeseGimBab(_name, _price);
	return gimbab;
}

CGimBabStore::Menu* CCheeseGimBabStore::MenuSelect()
{
	cout << "<<Ä¡Áî ±è¹ä ¸Þ´º >>" << endl;
	
	for (int i = 0; i < mCount; i++)
	{
		cout << i + 1 << ". " << mCheeseGimbabMenu[i]->gimbab_name << "  °¡°Ý:" << mCheeseGimbabMenu[i]->price << endl;
	}

	int select;
	cin >> select;

	return mCheeseGimbabMenu[select-1];
}

CGimBab* CCheeseGimBabStore::WrapGimBab(CGimBab* _gimbab)
{
	cout << "Ä¡Áî ±×¸²ÀÇ Æ÷ÀåÁö·Î ±è¹äÀ» ½Î°í Ä¡Áî½ºÆ¼°Å¸¦ ºÙÀÔ´Ï´Ù. " << endl;
	return _gimbab;
}