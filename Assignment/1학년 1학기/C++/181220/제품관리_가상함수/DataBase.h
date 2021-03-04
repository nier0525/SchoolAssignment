#ifndef DATABASE_H
#define DATABASE_H

#include "Product.h"
#include "Food.h"
#include "Wear.h"
#include "Appliance.h"

class cData {
private:
	CProduct* mPro;
	CFood* mFood;
	CWear* mWear;
	CAppliance*	mApp;
public:
	cData();
	~cData();

	void SearchData();
};

#endif