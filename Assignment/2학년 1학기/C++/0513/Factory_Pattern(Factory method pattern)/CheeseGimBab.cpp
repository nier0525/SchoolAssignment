#include "CheeseGimBab.h"
CCheeseGimBab::CCheeseGimBab(const char* _name, int _price):CGimBab(_name, _price)
{
}

CCheeseGimBab::~CCheeseGimBab()
{

}

void CCheeseGimBab::Prepare()
{
	cout << "±èÀ» ±ò°í ¹äÀ» ¿Ã·Á Æð´Ï´Ù....." << endl;
}

void CCheeseGimBab::IngredientSet()
{
	cout << "´Ü¹«Áö, ÇÜ, Ä¡Áî¸¦ ¿Ã¸³´Ï´Ù......" << endl;
}

void CCheeseGimBab::Roll()
{
	cout << "±è¹äÀ» ¸»¾Æ ´­·¯ÁÝ´Ï´Ù...." << endl;
}

void CCheeseGimBab::Cut()
{
	cout << "±è¹äÀ» Àû´çÇÑ Å©±â·Î ½ç´Ï´Ù....." << endl;
}