#include "CheeseGimBab.h"
CCheeseGimBab::CCheeseGimBab(const char* _name, int _price):CGimBab(_name, _price)
{
}

CCheeseGimBab::~CCheeseGimBab()
{

}

void CCheeseGimBab::Prepare()
{
	cout << "���� ��� ���� �÷� ��ϴ�....." << endl;
}

void CCheeseGimBab::IngredientSet()
{
	cout << "�ܹ���, ��, ġ� �ø��ϴ�......" << endl;
}

void CCheeseGimBab::Roll()
{
	cout << "����� ���� �����ݴϴ�...." << endl;
}

void CCheeseGimBab::Cut()
{
	cout << "����� ������ ũ��� ��ϴ�....." << endl;
}