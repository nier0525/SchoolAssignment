#pragma once

#include "Product.h"

class CFood : public CProduct {
private:
	char gram[CHARSIZE];
public:
	CFood(const char*, const char*, const char*, int);
	~CFood();

	void Setgram(const char*);
	const char* Getgram();
};