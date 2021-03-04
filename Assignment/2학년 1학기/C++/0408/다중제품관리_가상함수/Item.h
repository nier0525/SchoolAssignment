#pragma once

#include "Product.h"

class CItem : public CProduct {
private:
	char id[CHARSIZE];
public:
	CItem(const char*, const char*, const char*, int);
	~CItem();

	void Setid(const char*);
	const char* Getid();
};