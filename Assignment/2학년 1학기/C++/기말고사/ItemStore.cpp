#include "ItemStore.h"
IMPLEMENT_SINGLETON(ItemStore)
// �̱��� ��ũ�� ����

// ������ ����Ʈ �ʱ�ȭ
void ItemStore::Init() {
	mItemInfo[mItemInfoCount++] = new Wenpon("Diamond Sword", 3000, 75);
	mItemInfo[mItemInfoCount++] = new Armor("Diamond Shield", 2000, 30);
	mItemInfo[mItemInfoCount++] = new Accessories("Balance Chain", 3000, 150, 150);
	mItemInfo[mItemInfoCount++] = new Wenpon("Dragon Bow", 5000, 125);
	mItemInfo[mItemInfoCount++] = new Armor("Timeless Shield", 4000, 60);
	mItemInfo[mItemInfoCount++] = new Accessories("Heaven Ring", 5000, 300, 300);
}

// ������ �Ǹ� (���� ��������)
bool ItemStore::Sell(const char* name) {
	Character* now = CharacterManager::GetInstance()->GetNowCharacter();
	// ���� �������� ĳ���� ����

	// ������ ����Ʈ ��ȸ
	for (int i = 0; i < mItemInfoCount; i++) {

		if (!strcmp(mItemInfo[i]->GetItemName(), name)) { // ������ ��� �߿� ĳ���Ͱ� ã�� �������� �ִٸ�
			Item* item = nullptr; // �ӽ� ��ü

			// ���̳��� ĳ��Ʈ�� ������ �к��ϰ�, �׿� �´� ���⸦ �ӽ� ��ü�� �����Ѵ�.
			if (dynamic_cast<Wenpon*>(mItemInfo[i])) {
				item = new Wenpon(mItemInfo[i]->GetItemName(), mItemInfo[i]->GetPrice(), mItemInfo[i]->GetFirstAbility());
			}
			if (dynamic_cast<Armor*>(mItemInfo[i])) {
				item = new Armor(mItemInfo[i]->GetItemName(), mItemInfo[i]->GetPrice(), mItemInfo[i]->GetFirstAbility());
			}
			if (dynamic_cast<Accessories*>(mItemInfo[i])) {
				item = new Accessories(mItemInfo[i]->GetItemName(), mItemInfo[i]->GetPrice(), mItemInfo[i]->GetFirstAbility(),mItemInfo[i]->GetSecordAbility());
			}

			// ĳ���Ͱ� �� �������� ���� �����ϰ� �����Ѵ�.
			// ���� ���� �����ϴٸ� ������ �� ����.
			if (now->Buy(item)) {
				return true;
			}
		}
	}

	// �������� ��ã�Ұų�, ���� ������ ��
	return false;
}

// ������ ���� (���� ��������)
bool ItemStore::Buy(const char* name) {
	Character* now = CharacterManager::GetInstance()->GetNowCharacter();
	// ���� �������� ĳ���� ����

	Item* item = nullptr;
	while (now->GetItem(&item)) {	// ���� ĳ������ �κ��丮 ��ȸ
		if (!strcmp(item->GetItemName(), name)) {	// �Ű� ������ �̸��� ��ġ�ϴ� �������� �ִٸ�
			now->Sell(item);	// ĳ���Ͱ� �������� �Ǵ�.
			now->SearchEnd();	// ��� ��ġ �ʱ�ȭ
			return true;
		}
	}

	// �������� ã�� ����.
	now->SearchEnd();	// ��� ��ġ �ʱ�ȭ
	return false;
}

// ������ ����Ʈ ���
// ĳ���� �Ŵ����� ���������� �ܺο� ������ ����Ʈ�� ����ϱ� ���� �Լ��̴�.
// ������ Ŭ�������� �к��ϱ� ���� ���̳��� ĳ��Ʈ�� ����ߴ�.

void ItemStore::ShowItemInfo(HDC hdc, int x, int y) {
	char name[CHAR];
	char price[CHAR];
	char ability[CHAR];
	char ability2[CHAR];

	for (int i = 0; i < mItemInfoCount; i++) {

		wsprintf(name, mItemInfo[i]->GetItemName());
		wsprintf(price, "%d", mItemInfo[i]->GetPrice());
		wsprintf(ability, "%d", mItemInfo[i]->GetFirstAbility());
		wsprintf(ability2, "%d", mItemInfo[i]->GetSecordAbility());

		if (dynamic_cast<Wenpon*>(mItemInfo[i])) {
			TextOut(hdc, 220, 230 + (20 * i), name, strlen(name));
			TextOut(hdc, 380, 230 + (20 * i), price, strlen(price));
			TextOut(hdc, 510, 230 + (20 * i), TEXT("Att + "), 6);
			TextOut(hdc, 540, 230 + (20 * i), ability, strlen(ability));
		}
		if (dynamic_cast<Armor*>(mItemInfo[i])) {
			TextOut(hdc, 220, 230 + (20 * i), name, strlen(name));
			TextOut(hdc, 380, 230 + (20 * i), price, strlen(price));
			TextOut(hdc, 500, 230 + (20 * i), TEXT("Def + "), 6);
			TextOut(hdc, 540, 230 + (20 * i), ability, strlen(ability));
		}
		if (dynamic_cast<Accessories*>(mItemInfo[i])) {
			TextOut(hdc, 220, 230 + (20 * i), name, strlen(name));
			TextOut(hdc, 380, 230 + (20 * i), price, strlen(price));
			TextOut(hdc, 450, 230 + (20 * i), TEXT("HP / MP + "), 10);
			TextOut(hdc, 520, 230 + (20 * i), ability, strlen(ability));
			TextOut(hdc, 550, 230 + (20 * i), TEXT(" / "), 3);
			TextOut(hdc, 560, 230 + (20 * i), ability2, strlen(ability2));
		}
	}
}
