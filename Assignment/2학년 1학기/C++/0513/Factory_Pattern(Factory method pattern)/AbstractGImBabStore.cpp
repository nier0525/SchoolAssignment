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
		cout << "�߸� �Է��߽��ϴ�." << endl;
		break;
	}
	return store;
}