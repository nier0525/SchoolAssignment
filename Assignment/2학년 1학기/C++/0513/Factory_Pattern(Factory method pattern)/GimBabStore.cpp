#include "GimBabStore.h"
CGimBabStore::CGimBabStore()
{

}
CGimBabStore::~CGimBabStore()
{

}

CGimBab* CGimBabStore::OrderGimBab()
{
	
	Menu* menu = MenuSelect();

	CGimBab* gimbab = CreateGimBab(menu->gimbab_name, menu->price);

	gimbab->Prepare();
	gimbab->IngredientSet();
	gimbab->Roll();
	gimbab->Cut();

	gimbab = WrapGimBab(gimbab);

	cout << gimbab->GetName() << "�� �ϼ��Ǿ����ϴ�." << endl;
	return gimbab;
}
