#include "PenColor.h"

RedPen* RedPen::pthis = nullptr;

RedPen* RedPen::GetInstance() {
	if (!pthis) {
		pthis = new RedPen();
	}
	return pthis;
}

void RedPen::Destory() {
	if (pthis) {
		delete pthis;
	}
}



BluePen* BluePen::pthis = nullptr;

BluePen* BluePen::GetInstance() {
	if (!pthis) {
		pthis = new BluePen();
	}
	return pthis;
}

void BluePen::Destory() {
	if (pthis) {
		delete pthis;
	}
}



GreenPen* GreenPen::pthis = nullptr;

GreenPen* GreenPen::GetInstance() {
	if (!pthis) {
		pthis = new GreenPen();
	}
	return pthis;
}

void GreenPen::Destory() {
	if (pthis) {
		delete pthis;
	}
}
