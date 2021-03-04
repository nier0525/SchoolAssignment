#include "CheeseGimBabStore.h"

CCheeseGimBabStore::CCheeseGimBabStore()
{
	memset(mCheeseGimbabMenu, 0, sizeof(mCheeseGimbabMenu));
	mCount = 0;

	mCheeseGimbabMenu[mCount++] = new CGimBabStore::Menu("��¥���� ġ����", 3000);
	mCheeseGimbabMenu[mCount++] = new CGimBabStore::Menu("ũ�� ġ�� ���", 4000);
	mCheeseGimbabMenu[mCount++] = new CGimBabStore::Menu("ü�� ġ�� ���", 3500);

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

	cout << "ġ�� ��䰡�԰� ���� �ݽ��ϴ�. " << endl;
}

CGimBab* CCheeseGimBabStore::CreateGimBab(const char* _name, int _price)
{
	CGimBab* gimbab = new CCheeseGimBab(_name, _price);
	return gimbab;
}

CGimBabStore::Menu* CCheeseGimBabStore::MenuSelect()
{
	cout << "<<ġ�� ��� �޴� >>" << endl;
	
	for (int i = 0; i < mCount; i++)
	{
		cout << i + 1 << ". " << mCheeseGimbabMenu[i]->gimbab_name << "  ����:" << mCheeseGimbabMenu[i]->price << endl;
	}

	int select;
	cin >> select;

	return mCheeseGimbabMenu[select-1];
}

CGimBab* CCheeseGimBabStore::WrapGimBab(CGimBab* _gimbab)
{
	cout << "ġ�� �׸��� �������� ����� �ΰ� ġ�Ƽ�Ÿ� ���Դϴ�. " << endl;
	return _gimbab;
}