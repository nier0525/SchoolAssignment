#include "VegetableGimBab.h"

CVegetableGimBab::CVegetableGimBab(const char* _name, int _price):CGimBab(_name, _price)
{
}
CVegetableGimBab::~CVegetableGimBab()
{
}
void CVegetableGimBab::Prepare()
{
	cout << "���� ��� ���� �÷� ��ϴ�....." << endl;
}

void CVegetableGimBab::IngredientSet()
{
	cout << "�ܹ���, ��, ���, ������ �ø��ϴ�......" << endl;
}

void CVegetableGimBab::Roll()
{
	cout << "����� ���� �����ݴϴ�...." << endl;
}

void CVegetableGimBab::Cut()
{
	cout << "����� ������ ũ��� ��ϴ�....." << endl;
}