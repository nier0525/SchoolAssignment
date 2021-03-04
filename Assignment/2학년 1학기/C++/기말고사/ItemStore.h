#pragma once

#include "Wenpon.h"
#include "Armor.h"
#include "Accessories.h"

#include "CharacterManager.h"

// ������ ������ ����ϴ� Ŭ����
// ������ ����� �迭�� �޴�ó�� ��������
// �̱��� ��ũ��

class ItemStore {
	DECLARE_SINGLETONE(ItemStore)
private:
	// ������ �ʱ�ȭ �۾�
	ItemStore() {
		memset(mItemInfo, 0, sizeof(mItemInfo));
		mItemInfoCount = NODATA;
		mItemInfoIndex = NODATA;
	}
	// ������ ������ �迭 �޸� ����
	~ItemStore() {
		for (int i = 0; i < mItemInfoCount; i++) {
			delete mItemInfo[i];
		}
	}

	Item* mItemInfo[MAXITEM];	// ������ ����Ʈ
	int mItemInfoCount;			// ���� ������ ����Ʈ ����
	int mItemInfoIndex;			// ������ ����Ʈ ��ȸ
public:

	// �ʱ�ȭ
	void Init();

	// �Ǹ� �� ������ ���
	bool Sell(const char*);
	bool Buy(const char*);

	// ������ ��� ���
	void ShowItemInfo(HDC, int, int);

	// �б� ������
	Item* GetItemInfo(int index) { return mItemInfo[index]; }
	int GetItemInfoCount() { return mItemInfoCount; }
	int GetItemInfoIndex() { return mItemInfoIndex; }
};