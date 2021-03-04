#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
using namespace std;

#define NAMESIZE 128
#define PASSSIZE 128
#define USERSIZE 10

#define MAX 100
#define NODATA -1

enum Main {
	JOIN = 1, LOGIN, EXIT
};
enum Login {
	GAMESTART = 1, GAMESCORE , LOGOUT, DEL
};

#endif