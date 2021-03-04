#include "WearManager.h"

const char* cWearManager::mWearName[] = { "����","����","����" };

const char* cWearManager::mWearSex[] = { "����", "����" };

cWearManager::cWearManager() {
	memset(mWear, 0, sizeof(mWear));
	mWearCount = 0;
}

cWearManager::~cWearManager() {
	for (int i = 0; i < mWearCount; i++) {
		delete mWear[i];
	}
}

void cWearManager::Show() {
	for (int i = 0; i < mWearCount; i++) {
		cout << "��ǰ�� : " << mWear[i]->GetWearName() << endl;
		cout << "���� : " << mWear[i]->GetSex() << "\t";
		cout << "������ : " << mWear[i]->GetSize() << endl;
		cout << "�ܰ� : " << mWear[i]->GetMoney() << "\t";
		cout << "���� : " << mWear[i]->GetCount() << endl;
	}
}

void cWearManager::Input() {
	int number;
	int price;
	int count;
	int rank;
	int gram;

	while (1) {
		cout << "<<��ǰ����Ʈ>>" << endl;
		for (int i = 0; i < (sizeof(mWearName) / sizeof(char*)); i++) {
			cout << i + 1 << ". " << mWearName[i] << endl;
		}
		cout << "��ǰ ��ȣ : "; cin >> number;
		cout << "���� : ";	cin >> count;

		bool flag = false;

		for (int i = 0; i < mWearCount; i++) {
			if (!strcmp(mWear[i]->GetWearName(), mWearName[number - 1])) {
				int result = mWear[i]->InCount(count);
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
			cout << "����(1:����,2:����) : ";
			cin >> rank;
			cout << "����(1:90 ~ 5:110) : ";
			cin >> gram;

			cWear* Wear = new cWear();
			if (Wear->SetWearName(mWearName[number - 1]) &&
				Wear->SetSex(mWearSex[rank - 1]) && 
				Wear->SetMoney(price) &&
				Wear->SetSize(gram) &&  
				Wear->SetCount(count)) 
			{
				mWear[mWearCount++] = Wear;
			}
			else {
				cout << "��ǰ ��� ������ ���� �ʽ��ϴ�." << endl;
				delete Wear;
				continue;
			}
		}
		break;
	}
}

void cWearManager::Output() {
	int count, number;
	while (1) {
		cout << "<<��ǰ����Ʈ>>" << endl;
		for (int i = 0; i < sizeof(mWearName) / sizeof(char*); i++) {
			cout << i + 1 << ". " << mWearName[i] << endl;
		}
		cout << "��ǰ��ȣ : ";
		cin >> number;

		bool flag = false;

		for (int i = 0; i < mWearCount; i++) {
			if (!strcmp(mWear[i]->GetWearName(), mWearName[number - 1])) {
				cout << "���� : ";	cin >> count;
				int result = mWear[i]->DeCount(count);
				cout << result << "���� ���Ǿ����ϴ�." << endl;
				if (count != result) {
					cout << "��� ������ ��ǰ�� �����ϴ�." << endl;
				}
				if (mWear[i]->GetCount() == 0) {
					delete mWear[i];
					for (int j = i; j < mWearCount - 1; j++) {
						mWear[j] = mWear[j + 1];
					}
					mWear[mWearCount - 1] = nullptr;
					mWearCount--;
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