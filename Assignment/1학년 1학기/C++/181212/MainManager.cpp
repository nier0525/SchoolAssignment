#include "MainManager.h"

cMain::cMain() {
	mFoodMng = nullptr;
	mWearMng = nullptr;
	mItemMng = nullptr;
}

void cMain::Initialize() {
	mFoodMng = new cFoodManager();
	mWearMng = new cWearManager();
	mItemMng = new cItemManager();
}

cMain::~cMain() {
	if (mFoodMng != nullptr) {
		delete mFoodMng;
	}
	if (mWearMng != nullptr) {
		delete mWearMng;
	}
	if (mItemMng != nullptr) {
		delete mItemMng;
	}
}

int cMain::ShowMenu() {
	int sel;
	cout << "<��ǰ ���� ���α׷�>" << endl;
	cout << "1. ��ǰ �԰�" << endl;
	cout << "2. ��ǰ ���" << endl;
	cout << "3. ��ǰ ��ȸ" << endl;
	cout << "4. ����" << endl;
	cout << "���� : ";	cin >> sel;
	return sel;
}

void cMain::Run() {
	int sel;
	Initialize();

	while (1) {
		sel = ShowMenu();

		switch (sel) {
		case INPUT:
			MajorIuput();
			break;
		case OUTPUT:
			MajorOutput();
			break;
		case SHOW:
			MajorShow();
			break;
		case EXIT:
			return;
		default:
			cout << "�߸� �Է��߽��ϴ�. �ٽ� �Է��ϼ���." << endl;
			continue;
		}
	}
}

void cMain::MajorIuput() {
	while (1) {
		cout << "<��ǰ : 1 | �� : 2 | ������ǰ : 3>" << endl;
		cout << "���� : ";
		int sel;
		cin >> sel;

		switch (sel) {
		case FOOD:
			mFoodMng->Input();
			break;
		case WEAR:
			mWearMng->Input();
			break;
		case APP:
			mItemMng->Input();
			break;
		default:
			cout << "�߸� �Է��߽��ϴ�. �ٽ� �Է��ϼ���." << endl;
			continue;
		}
		break;
	}
}

void cMain::MajorOutput() {
	while (1) {
		cout << "<��ǰ : 1 | �� : 2 | ������ǰ : 3>" << endl;
		cout << "���� : ";
		int sel;
		cin >> sel;

		switch (sel) {
		case FOOD:
			mFoodMng->Output();
			break;
		case WEAR:
			mWearMng->Output();
			break;
		case APP:
			mItemMng->Output();
			break;
		default:
			cout << "�߸� �Է��߽��ϴ�. �ٽ� �Է��ϼ���." << endl;
			continue;
		}
		break;
	}
}

void cMain::MajorShow() {
	while (1) {
		cout << "<��ǰ : 1 | �� : 2 | ������ǰ : 3>" << endl;
		cout << "���� : ";
		int sel;
		cin >> sel;

		switch (sel) {
		case FOOD:
			mFoodMng->Show();
			break;
		case WEAR:
			mWearMng->Show();
			break;
		case APP:
			mItemMng->Show();
			break;
		default:
			cout << "�߸� �Է��߽��ϴ�. �ٽ� �Է��ϼ���." << endl;
			continue;
		}
		break;
	}
}