#pragma once

#include "Wenpon.h"
#include "Armor.h"
#include "Accessories.h"

// ���� ���丮 Ŭ����
// ���� �������� �ٸ� ���⸦ �����ϱ� ���� Ŭ������.
// ��������Ŭ����

class KindFactory {
private:

public:
	KindFactory() {}
	virtual ~KindFactory() {}

	virtual Wenpon* CreateWenpon() = 0;
	virtual Armor* CreateArmor() = 0;
	virtual Accessories* CreteAccessories() = 0;
};