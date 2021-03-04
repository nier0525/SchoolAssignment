#include "ItemFactory.h"

ItemFactory::ItemFactory() {

}

ItemFactory::~ItemFactory() {

}

Item* ItemFactory::Sell(int sel) {

	item = nullptr;

	switch (sel) {
	case WENPON:
		item = CreateWenpon();
		break;

	case ARMOR:
		item = CreateArmor();
		break;

	case ACCESSORIES:
		item = CreateAccessories();
		break;
	}
	item->MakeCode();
	return item;
}

Item* ItemFactory::GetItem() {
	return item;
}