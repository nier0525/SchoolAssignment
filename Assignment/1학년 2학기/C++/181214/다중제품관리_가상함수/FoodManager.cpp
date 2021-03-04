#include "FoodManager.h"

const char* cFoodManager::mFoodName[] = { "�Ұ��","�������","����" };

cFoodManager::cFoodManager() {
	memset(mFood, 0, sizeof(mFood));
	mFoodCount = 0;
}

cFoodManager::~cFoodManager() {
	for (int i = 0; i < mFoodCount; i++) {
		delete mFood[i];
	}
}

void cFoodManager::Show() {
	for (int i = 0; i < mFoodCount; i++) {
		cout << "��ǰ�� : " << mFood[i]->GetFoodName() << endl;
		cout << "ǰ�� : " << mFood[i]->GetRank() << "\t";
		cout << "���� : " << mFood[i]->GetGram() << endl;
		cout << "�ܰ� : " << mFood[i]->GetMoney() << "\t";
		cout << "���� : " << mFood[i]->GetCount() << endl;
	}
}

void cFoodManager::Input() {
	int number;
	int price;
	int count;
	int rank;
	int gram;

	while (1) {
		cout << "<<��ǰ����Ʈ>>" << endl;
		for (int i = 0; i < (sizeof(mFoodName) / sizeof(char*)); i++) {
			cout << i + 1 << ". " << mFoodName[i] << endl;
		}
		cout << "��ǰ ��ȣ : "; cin >> number;
		cout << "���� : ";	cin >> count;

		bool flag = false;

		for (int i = 0; i < mFoodCount; i++) {
			if (!strcmp(mFood[i]->GetFoodName(), mFoodName[number - 1])) {
				int result = mFood[i]->InCount(count);
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
			cout << "ǰ��(1 ~ 5) : ";
			cin >> rank;
			cout << "���� : ";
			cin >> gram;

			cFood* Food = new cFood();
			if (Food->SetFoodName(mFoodName[number - 1]) &&
				Food->SetMoney(price) && Food->SetRand(rank) &&
				Food->SetGram(gram) && Food->SetCount(count)) {
				mFood[mFoodCount++] = Food;
			}
			else {
				cout << "��ǰ ��� ������ ���� �ʽ��ϴ�." << endl;
				delete Food;
				continue;
			}
		}
		break;
	}
}

void cFoodManager::Output() {
	int count, number;
	while (1) {
		cout << "<<��ǰ����Ʈ>>" << endl;
		for (int i = 0; i < sizeof(mFoodName) / sizeof(char*); i++) {
			cout << i + 1 << ". " << mFoodName[i] << endl;
		}
		cout << "��ǰ��ȣ : ";
		cin >> number;

		bool flag = false;

		for (int i = 0; i < mFoodCount; i++) {
			if (!strcmp(mFood[i]->GetFoodName(), mFoodName[number - 1])) {
				cout << "���� : ";	cin >> count;
				int result = mFood[i]->DeCount(count);
				cout << result << "���� ���Ǿ����ϴ�." << endl;
				if (count != result) {
					cout << "��� ������ ��ǰ�� �����ϴ�." << endl;
				}
				if (mFood[i]->GetCount() == 0) {
					delete mFood[i];
					for (int j = i; j < mFoodCount - 1; j++) {
						mFood[j] = mFood[j + 1];
					}
					mFood[mFoodCount - 1] = nullptr;
					mFoodCount--;
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