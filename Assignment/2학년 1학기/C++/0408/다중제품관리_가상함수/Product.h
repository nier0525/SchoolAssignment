#pragma once

#include "Global.h"

class CProduct {
private:
	char name[CHARSIZE];
	char makeday[CHARSIZE];
	char mid[CHARSIZE];
	int price;
public:
	CProduct(const char*, const char*, const char*, int);
	~CProduct();

	void Setname(const char*);
	void Setmakeday(const char*);
	void Setprice(int);
	void Setmid(const char*);

	const char* Getname();
	const char* Getmakeday();
	const char* Getmid();
	int Getprice();
};