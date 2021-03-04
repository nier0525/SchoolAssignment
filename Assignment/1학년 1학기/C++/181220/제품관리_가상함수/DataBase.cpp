#include "DataBase.h"

cData::cData() {
	mPro = nullptr;
	mFood = dynamic_cast<CFood*>(mPro);
	mApp = dynamic_cast<CAppliance*>(mPro);
	mWear = dynamic_cast<CWear*>(mPro);
}

cData::~cData() {}

void cData::SearchData() {
	while (1) {
		mPro->MakeId();
	}
}