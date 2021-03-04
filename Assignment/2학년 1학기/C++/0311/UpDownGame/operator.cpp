#include "Global.h"

istream& operator>>(istream& i, MAIN_MENU& input)
{
	scanf("%d", &input);
	return i;
}

istream& operator>>(istream& i, LOGIN_MENU& input)
{
	scanf("%d", &input);
	return i;
}