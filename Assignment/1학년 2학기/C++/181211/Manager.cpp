#include "Manager.h"

cManager::cManager() {
	for (int i = 0; i < ASIZE; i++) {
		mFruit[i] = new cFruit(); // ��ü ����
	}
}

cManager::~cManager() {
	for (int i = 0; i < ASIZE; i++) {
		delete mFruit[i]; // ����
	}
}

void cManager::Input() {
	int sel;
	int Mnum;
	int Cnum;
	int index;
	int dex;

	cout << "<��ǰ����Ʈ>" << endl;
	cout << "1. ���" << endl << "2. ��" << endl << "3. �ٳ���" << endl
		<< "4. �ڸ�" << endl << "5. ����" << endl << "��ȣ : ";
	cin >> sel; // ��ȣ

	cout << "���� : ";	cin >> Cnum; // ����
	dex = Cnum;

	if (mFruit[sel]->GetMoney() == 0) { // �ܰ� �Է��� ó���� ���
		cout << "�ܰ� : ";	cin >> Mnum;
		mFruit[sel]->SetMoney(Mnum);
	}
	index = mFruit[sel]->GetCount(); // index ���� �ʱ�ȭ

	if (Cnum && mFruit[sel]->GetCount() <= 10) { // �������� 10 ���� ������
		index += Cnum; // ��������ŭ ����
		mFruit[sel]->SetCount(index); // ���� �� ������ ��ü �Լ��� ����
	}

	if (Cnum && mFruit[sel]->GetCount() > 10 ) { // �������� 10 ���� ũ��
		Cnum = mFruit[sel]->GetCount() - (Cnum = 10); // ������� ���� ����..

		index -= Cnum; // 10�� �ʰ��ϴ� ������ŭ ���� = 10
		mFruit[sel]->SetCount(index); // ��� �� ��ü �Լ��� ����

		dex -= Cnum; // x + y �� y �� ���ϴ� ����
		cout << "��� ���� �ʰ�! �� �̻� �԰� �Ұ����մϴ�." << endl;
	}
	cout << dex << " ���� �԰�Ǿ����ϴ�." << endl;
}

void cManager::Output() {
	int sel;
	int Cnum;
	int index;
	int dex;

	cout << "<��ǰ����Ʈ>" << endl;
	cout << "1. ���" << endl << "2. ��" << endl << "3. �ٳ���" << endl
		<< "4. �ڸ�" << endl << "5. ����" << endl << "��ȣ : ";
	cin >> sel; // ��ȣ

	cout << "���� : ";	cin >> Cnum; // ����
	dex = Cnum;
	index = mFruit[sel]->GetCount(); // ���� �ʱ�ȭ

	index -= Cnum; // ��������ŭ ����
	mFruit[sel]->SetCount(index); // �ᰡ �� ��ü �Լ��� ����

	if (Cnum && mFruit[sel]->GetCount() <= 0) { // 0 ���� ������
		dex = index += Cnum; // ������ �� ���� ����
		mFruit[sel]->SetCount(0); // 0 ���� �ʱ�ȭ
		cout << "��� �����ϴ�! �� �̻� ��� �Ұ����մϴ�." << endl;
	}

	cout << dex << " ���� ���Ǿ����ϴ�." << endl;
}

void cManager::Show() {
	for (int i = 0; i < ASIZE; i++) { // ����� ���� for ��
		switch (i) {
		case APPLE:
			if (mFruit[APPLE]->GetCount() != 0) {
				cout << "���" << endl;
				cout << "���� : " << mFruit[APPLE]->GetCount() << "\t" << "�ܰ� : " <<
					mFruit[APPLE]->GetMoney() << endl;
			}
			break;
		case BAE:
			if (mFruit[BAE]->GetCount() != 0) {
				cout << "��" << endl;
				cout << "���� : " << mFruit[BAE]->GetCount() << "\t" << "�ܰ� : " <<
					mFruit[BAE]->GetMoney() << endl;
			}
			break;
		case BANANA:
			if (mFruit[BANANA]->GetCount() != 0) {
				cout << "�ٳ���" << endl;
				cout << "���� : " << mFruit[BANANA]->GetCount() << "\t" << "�ܰ� : " <<
					mFruit[BANANA]->GetMoney() << endl;
			}
			break;
		case JAMONG:
			if (mFruit[JAMONG]->GetCount() != 0) {
				cout << "�ڸ�" << endl;
				cout << "���� : " << mFruit[JAMONG]->GetCount() << "\t" << "�ܰ� : " <<
					mFruit[JAMONG]->GetMoney() << endl;
			}
			break;
		case PODO:
			if (mFruit[PODO]->GetCount() != 0) {
				cout << "����" << endl;
				cout << "���� : " << mFruit[PODO]->GetCount() << "\t" << "�ܰ� : " <<
					mFruit[PODO]->GetMoney() << endl;
			}
			break;

		} // ���� ���
	}
}
 
void cManager::ShowMenu() {
	cout << "<��ǰ����>" << endl;
	cout << "1. ��ǰ�԰�\t 2. ��ǰ���\t 3. ��ǰ���\t 4. ���� " << endl;
	cout << "���� : ";
	// �޴� ��°�
}

void cManager::Run() {
	int sel;
	while (1) {
		ShowMenu();
		cin >> sel;
		switch (sel) {
		case INPUT:
			Input();
			break;
		case OUTPUT:
			Output();
			break;
		case SHOWDATA:
			Show();
			break;
		case EXIT:
			return;
		default:
			cout << "�߸� �Է��߽��ϴ�. �ٽ� �Է��ϼ���." << endl;
		}
	}
}