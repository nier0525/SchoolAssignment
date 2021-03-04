#include "AbstractGimBabStore.h"

CGimBabStore* AbstractGimBabStore::CreateGimBabStore(int _sel) {
	CGimBabStore* store = nullptr;
	switch (_sel) {
	case CHEESE:
		store = new CCheeseGimBabStore();
		break;

	case VEGETABLE:
		store = new CVegetableGimBabStore();
		break;

	default:
		cout << "잘못 입력했습니다." << endl;
		break;
	}
	return store;
}