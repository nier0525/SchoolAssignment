#pragma once

#include <iostream>
using namespace std;

#define CHARSIZE 128

#define MAXCOUNT 10
#define GAMEMONEY 5000

#define NODATA 0
#define LISTSIZE 10

enum LOGINMENU { NEWLOG = 1, LOGIN , EXIT };
enum GAMEMENU { NEW = 1, DEL , SELECT , BUY , SELL , LOGOUT , DELLOG };

enum CHARACTERMENU { ELF = 1, OAK , HUMAN };
enum ITEMMENU { WENPON = 1, ARMOR , ACCESSORIES };
