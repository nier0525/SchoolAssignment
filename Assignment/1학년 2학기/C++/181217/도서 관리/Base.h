#ifndef BASE_H
#define BASE_H
#include <iostream>
using namespace std;

#define ID 1000
#define PASSWORD 1000
#define MAXID	10

#define BOOKSIZE 1000
#define BOOKNAME 1000
#define BOOKCODE 200
#define BOOKCOUNT 30

#define BORROW_MAX 3

enum { NEW = 1, ROGIN, HOST , EXIT };										// �޴�
enum { DISPLAY = 1, BOOKSERACH , BOOKINPUT, BOOKRETURN , ROGOUT, GETROG };	// �����뿩
enum { INPUT = 1 , OUTPUT, RETURN };										// ������ ���

#endif