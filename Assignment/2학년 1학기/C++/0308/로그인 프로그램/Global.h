#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
using namespace std;

#define NAMESIZE 128
#define PASSSIZE 128
#define USERSIZE 10

enum Main {
	JOIN = 1, LOGIN, EXIT
};
enum Login {
	LOGOUT = 1, DEL
};

#endif