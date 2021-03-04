#include "CManager.h"

const char* cManager::mNameList[] = { "���","��","�ٳ���","�ڸ�","����" };

cManager::cManager() {
	memset(ListProduct, 0, sizeof(ListProduct));
	mListCount = 0;
	product = nullptr;
}

cManager::~cManager() {
	for (int i = 0; i < mListCount; i++) {
		if (ListProduct[i] != nullptr) {
			delete ListProduct[i];
			ListProduct[i] = nullptr;
		}
	}
	delete product;	
}

void cManager::CinCheak() {
	if (cin.failbit) {
		cin.clear();
		cin.ignore(256, '\n'); // while (getchar() != '\n');
	}
}

void cManager::Run() {
	while (1) {
		int sel;

		cout << "< ��ǰ���� �ý��� > " << endl << "1. ��ǰ �԰�" << endl << "2. ��ǰ ���" << endl << "3. ��ǰ ���" << endl << "4 .����" << endl;
		cout << "���� : ";
		cin >> sel;

		switch (sel) {
		case INPUT:
			Add();
			break;

		case OUTPUT:
			Del();
			break;

		case DISPLAY:
			Display();
			break;

		case EXIT:
			return;
			break;
		}
	}
}

void cManager::Add() {
	int numb;
	int count;
	int money;

	bool flug = false;

	while (1) {

		cout << "< ��ǰ ����Ʈ >" << endl;

		for (int i = 0; i < (sizeof(mNameList) / sizeof(char*)); i++) {
			cout << i + 1 << ". " << mNameList[i] << endl;
		}

		cout << "��ǰ ��ȣ : ";
		cin >> numb;
		CinCheak();

		cout << "��ǰ ���� : ";
		cin >> count;
		CinCheak();

		for (int i = 0; i < mListCount; i++) {
			if (!strcmp(ListProduct[i]->GetName(), mNameList[numb - 1])) {
				int index = ListProduct[i]->Add(count);
				cout << index << " ���� �԰�Ǿ����ϴ�." << endl;
				
				if (index != count) {
					cout << "�� �̻� �԰� �Ұ����մϴ�." << endl;
				}
				flug = true;
				break;
			}
		}

		if (!flug) {
			cout << "��ǰ ���� : ";
			cin >> money;
			CinCheak();

			product = new cProduct();

			if (product->SetName(mNameList[numb - 1]) && product->SetCount(count) && product->SetMoney(money)) {
				ListProduct[mListCount++] = product;
				product = nullptr;
			}
			else {
				cout << "��ǰ ��� ������ ���� �ʽ��ϴ�." << endl;
				delete product;
				continue;
			}
		}
		break;
	}
}

void cManager::Del() {
	int count;
	int numb;

	bool flug = false;

	while (1) {
		cout << "< ��ǰ ����Ʈ >" << endl;

		for (int i = 0; i < (sizeof(mNameList) / sizeof(char*)); i++) {
			cout << i + 1 << ". " << mNameList[i] << endl;
		}

		cout << "��ǰ ��ȣ : ";
		cin >> numb;
		CinCheak();

		for (int i = 0; i < mListCount; i++) {
			if (!strcmp(ListProduct[i]->GetName(), mNameList[numb - 1])) {

				cout << "��ǰ ���� : ";
				cin >> count;
				CinCheak();


				int index = ListProduct[i]->Delete(count);
				cout << index << " ���� ��� �Ǿ����ϴ�." << endl;
				
				if (count != index) {
					cout << "��� ������ ��ǰ�� �����ϴ�." << endl;
				}

				if (ListProduct[i]->GetCount() == 0) {
					delete ListProduct[i];
					for (int j = i; j < mListCount - 1; j++) {
						ListProduct[j] = ListProduct[j + 1];
					}
					ListProduct[mListCount - 1] = nullptr;
					mListCount--;
				}
				flug = true;
				break;
			}
		}
		if (!flug) {
			cout << "���� ��ǰ�Դϴ�. �ٽ� �Է��Ͻðڽ��ϱ�?" << endl;
			int sel;
			cout << "��(1) / �ƴϿ�(2) : ";
			cin >> sel;

			if (sel == 1) {
				continue;
			}
		}
		break;
	}
}

void cManager::Display() {
	for (int i = 0; i < mListCount; i++) {
		cout << ListProduct[i]->GetName() << endl;
		cout << "���� : " << ListProduct[i]->GetCount() << endl;
		cout << "���� : " << ListProduct[i]->GetMoney() << endl;
	}
	if (mListCount == 0) {
		cout << "���� ��ǰ�� ��� �����ϴ�." << endl;
	}
}