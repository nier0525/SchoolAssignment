#ifndef BASE_H
#define BASE_H
#include <iostream>
using namespace std;

#define ID 10000
#define PASSWORD 10000
#define MAXID	10

#define BOOKSIZE 100
#define BOOKNAME 30000
#define BOOKCODE 200
#define BOOKCOUNT 10

enum { NEW = 1, ROGIN, HOST , EXIT };										// �޴�
enum { BOOKSERACH = 1, BOOKINPUT, BOOKRETURN , ROGOUT, GETROG };	// �����뿩
enum { INPUT = 1 , OUTPUT, RETURN };										// ������ ���

#endif