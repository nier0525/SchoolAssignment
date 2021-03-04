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

	cout << gimbab->GetName() << "이 완성되었습니다." << endl;
	return gimbab;
}
