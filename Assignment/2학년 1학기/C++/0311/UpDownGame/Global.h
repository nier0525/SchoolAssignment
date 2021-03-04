#ifndef  GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <time.h>

using namespace std;

#define NODATA -1

enum MAIN_MENU
{
	JOIN = 1,
	LOGIN,
	EXIT
};

enum LOGIN_MENU
{
	GAMESTART = 1,
	LOGOUT,
	DROP
};

istream& operator>>(istream& i, MAIN_MENU& input);

istream& operator>>(istream& i, LOGIN_MENU& input);


#endif // ! _GLOBAL_H
