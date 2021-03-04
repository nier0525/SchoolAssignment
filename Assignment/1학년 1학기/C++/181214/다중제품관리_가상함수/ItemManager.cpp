#include "ItemManager.h"

const char* cItemManager::mItemName[] = { "����Ʈ","��ǻ��","����" };

cItemManager::cItemManager() {
	memset(mItem, 0, sizeof(mItem));
	mItemCount = 0;
}

cItemManager::~cItemManager() {
	for (int i = 0; i < mItemCount; i++) {
		delete mItem[i];
	}
}

void cItemManager::Show() {
	for (int i = 0; i < mItemCount; i++) {
		cout << "��ǰ�� : " << mItem[i]->GetItemName() << endl;
		cout << "�Ϸù�ȣ : 20181213" << mItem[i]->GetItemid() << "\t";
		cout << "��������Ⱓ(��) : " << mItem[i]->GetFreeAs() << endl;
		cout << "�ܰ� : " << mItem[i]->GetMoney() << "\t";
		cout << "���� : " << mItem[i]->GetCount() << endl;
	}
}

void cItemManager::Input() {
	int number;
	int price;
	int count;
	int rank;
	int gram;

	while (1) {
		cout << "<<��ǰ����Ʈ>>" << endl;
		for (int i = 0; i < (sizeof(mItemName) / sizeof(char*)); i++) {
			cout << i + 1 << ". " << mItemName[i] << endl;
		}
		cout << "��ǰ ��ȣ : "; cin >> number;
		cout << "���� : ";	cin >> count;

		bool flag = false;

		for (int i = 0; i < mItemCount; i++) {
			if (!strcmp(mItem[i]->GetItemName(), mItemName[number - 1])) {
				int result = mItem[i]->InCount(count);
				cout << result << "���� �԰�Ǿ����ϴ�." << endl;
				if (count != result) {
					cout << "�԰� ������ ������ �� á���ϴ�. �� �̻� �԰� �Ұ����մϴ�." << endl;
				}
				flag = true;
				break;
			}
		}
		if (!flag) {
			cout << "�ܰ� : ";
			cin >> price;
			cout << "�Ϸù�ȣ : ";
			cin >> rank;
			cout << "��������Ⱓ(�ִ�24����) : ";
			cin >> gram;

			cItem* Item = new cItem();
			if (Item->SetItemName(mItemName[number - 1]) &&
				Item->SetMoney(price) && Item->SetItemid(rank) &&
				Item->SetFreeAs(gram) && Item->SetCount(count)) {
				mItem[mItemCount++] = Item;
			}
			else {
				cout << "��ǰ ��� ������ ���� �ʽ��ϴ�." << endl;
				delete Item;
				continue;
			}
		}
		break;
	}
}

void cItemManager::Output() {
	int count, number;
	while (1) {
		cout << "<<��ǰ����Ʈ>>" << endl;
		for (int i = 0; i < sizeof(mItemName) / sizeof(char*); i++) {
			cout << i + 1 << ". " << mItemName[i] << endl;
		}
		cout << "��ǰ��ȣ : ";
		cin >> number;

		bool flag = false;

		for (int i = 0; i < mItemCount; i++) {
			if (!strcmp(mItem[i]->GetItemName(), mItemName[number - 1])) {
				cout << "���� : ";	cin >> count;
				int result = mItem[i]->DeCount(count);
				cout << result << "���� ���Ǿ����ϴ�." << endl;
				if (count != result) {
					cout << "��� ������ ��ǰ�� �����ϴ�." << endl;
				}
				if (mItem[i]->GetCount() == 0) {
					delete mItem[i];
					for (int j = i; j < mItemCount - 1; j++) {
						mItem[j] = mItem[j + 1];
					}
					mItem[mItemCount - 1] = nullptr;
					mItemCount--;
				}
				flag = true;
				break;
			}
		}
		if (!flag) {
			cout << "���� ��ǰ�Դϴ�. �ٽ� �Է��ϼ���" << endl;
			int yn;
			cout << "�ٸ� ��� ��ǰ�� �Է��Ͻðڽ��ϱ�? (Yes : 1, No : 2)";
			cin >> yn;
			if (yn == 1) {
				continue;
			}
		}
		break;
	}
}