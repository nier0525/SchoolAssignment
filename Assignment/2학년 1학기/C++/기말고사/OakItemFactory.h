#pragma once

#include "KindFactory.h"

// ��ũ ������ ���丮 Ŭ����
// �̱��� ��ũ��

class OakItemFactory : public KindFactory {
	DECLARE_SINGLETONE(OakItemFactory)
private:
	OakItemFactory() {}
	virtual ~OakItemFactory() {}
public:
	// ���� ��� ����
	virtual Wenpon* CreateWenpon() {
		return new Wenpon("Steel Axe", 2000, 50);
	}

	virtual Armor* CreateArmor() {
		return new Armor("Steel Shield", 1500, 20);
	}

	virtual Accessories* CreteAccessories() {
		return new Accessories("Health Chain", 3000, 300, 0);
	}
};