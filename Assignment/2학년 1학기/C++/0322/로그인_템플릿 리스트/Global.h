#pragma once

#include <iostream>
using namespace std;

#define CHARSIZE 100
#define USERSIZE 10
#define NODATA 0

enum Main {
	JOIN = 1, LOGIN, EXIT
};

enum Login {
	LOGOUT = 1, DEL
};