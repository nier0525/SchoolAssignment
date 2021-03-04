#pragma once

#include "Product.h"

class CWear : public CProduct {
private:
	char size[CHARSIZE];
public:
	CWear(const char*, const char*, const char* , int);
	~CWear();

	void Setsize(const char*);
	const char* Getsize();
};