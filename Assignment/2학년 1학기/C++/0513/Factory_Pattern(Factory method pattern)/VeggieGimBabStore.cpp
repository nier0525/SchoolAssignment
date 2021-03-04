#include "VeggieGimBabStore.h"

CVegetableGimBabStore::CVegetableGimBabStore()
{
	memset(mVegetableGimbabMenu, 0, sizeof(mVegetableGimbabMenu));
	mCount = 0;

	mVegetableGimbabMenu[mCount++] = new CGimBabStore::Menu("��ٸ��� ��ä���", 3000);
	mVegetableGimbabMenu[mCount++] = new CGimBabStore::Menu("���ٸ��� ��ä���", 4000);
	mVegetableGimbabMenu[mCount++] = new CGimBabStore::Menu("���߸��� ��ä���", 3500);
}

CVegetableGimBabStore::~CVegetableGimBabStore()
{
	for (int i = 0; i < mCount; i++)
	{
		if (mVegetableGimbabMenu[i] != nullptr)
		{
			delete mVegetableGimbabMenu[i];
		}
	}

	cout << "��ä ��䰡�԰� ���� �ݽ��ϴ�. " << endl;
}

CGimBab* CVegetableGimBabStore::CreateGimBab(const char* _name, int _price)
{
	CGimBab* gimbab = new CVegetableGimBab(_name, _price);

	return gimbab;
}
 
CGimBabStore::Menu* CVegetableGimBabStore::MenuSelect()
{
	cout << "<<��ä ��� �޴� >>" << endl;

	for (int i = 0; i < mCount; i++)
	{
		cout << i + 1 << ". " << mVegetableGimbabMenu[i]->gimbab_name << "  ����:" << mVegetableGimbabMenu[i]->price << endl;
	}

	int select;
	cin >> select;

	return mVegetableGimbabMenu[select-1];

}
CGimBab* CVegetableGimBabStore::WrapGimBab(CGimBab* _gimbab)
{
	cout << "��ä �׸��� �������� ����� �ΰ� ��ä��Ƽ�Ÿ� ���Դϴ�. " << endl;
	return _gimbab;
}