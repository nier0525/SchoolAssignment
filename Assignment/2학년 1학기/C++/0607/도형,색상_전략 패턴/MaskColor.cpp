#include "MaskColor.h"

RedMask* RedMask::pthis = nullptr;

RedMask* RedMask::GetInstance() {
	if (!pthis) {
		pthis = new RedMask();
	}
	return pthis;
}

void RedMask::Destory() {
	if (pthis) {
		delete pthis;
	}
}



BlueMask* BlueMask::pthis = nullptr;

BlueMask* BlueMask::GetInstance() {
	if (!pthis) {
		pthis = new BlueMask();
	}
	return pthis;
}

void BlueMask::Destory() {
	if (pthis) {
		delete pthis;
	}
}



GreenMask* GreenMask::pthis = nullptr;

GreenMask* GreenMask::GetInstance() {
	if (!pthis) {
		pthis = new GreenMask();
	}
	return pthis;
}

void GreenMask::Destory() {
	if (pthis) {
		delete pthis;
	}
}