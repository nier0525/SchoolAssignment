#pragma once

#include <iostream>
using namespace std;

#define CHARSIZE 128

#define MAXCOUNT 10
#define GAMEMONEY 10000

#define NODATA 0
#define LISTFULL 10

enum LOGINMENU { NEW = 1, LOGIN , EXIT };
enum CHARACTERMENU { NEW_C = 1, SEL_C, DEL_C, LOGOUT , DELETE };
enum ITEMMENU { BUY = 1, SELL, PREV };

enum CHARACTER { ELF = 1, OAK , HUMAN };
enum ITEM {WENPON = 1, ARMOR , ACCESSORIES };
enum JOBMENU { WARRIOR = 1, ARCHER , WIZARD };