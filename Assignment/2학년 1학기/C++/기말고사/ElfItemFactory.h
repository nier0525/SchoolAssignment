#pragma once

#include "KindFactory.h"

// ���� ������ ���丮 Ŭ����
// �̱��� ��ũ��

class ElfItemFactory : public KindFactory {
	DECLARE_SINGLETONE(ElfItemFactory)
private:
	ElfItemFactory() {}
	virtual ~ElfItemFactory() {}
public:
	// ���� ��� ����
	virtual Wenpon* CreateWenpon() {
		return new Wenpon("Bronze Sword", 2000, 50);
	}

	virtual Armor* CreateArmor() {
		return new Armor("Bronze Shield", 1500, 20);
	}

	virtual Accessories* CreteAccessories() {
		return new Accessories("Magic Chain", 3000, 0, 300);
	}
};