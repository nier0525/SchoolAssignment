#pragma once

#include "VeggieGimBabStore.h"
#include "CheeseGimBabStore.h"

class AbstractGimBabStore {
private:

public:
	AbstractGimBabStore() {}
	~AbstractGimBabStore() {}

	CGimBabStore* CreateGimBabStore(int);
};