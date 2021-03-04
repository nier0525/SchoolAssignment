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
	cout << "<제품 관리 프로그램>" << endl;
	cout << "1. 제품 입고" << endl;
	cout << "2. 제품 출고" << endl;
	cout << "3. 제품 조회" << endl;
	cout << "4. 종료" << endl;
	cout << "선택 : ";	cin >> sel;
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
			cout << "잘못 입력했습니다. 다시 입력하세요." << endl;
			continue;
		}
	}
}

void cMain::MajorIuput() {
	while (1) {
		cout << "<식품 : 1 | 옷 : 2 | 가전제품 : 3>" << endl;
		cout << "선택 : ";
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
			cout << "잘못 입력했습니다. 다시 입력하세요." << endl;
			continue;
		}
		break;
	}
}

void cMain::MajorOutput() {
	while (1) {
		cout << "<식품 : 1 | 옷 : 2 | 가전제품 : 3>" << endl;
		cout << "선택 : ";
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
			cout << "잘못 입력했습니다. 다시 입력하세요." << endl;
			continue;
		}
		break;
	}
}

void cMain::MajorShow() {
	while (1) {
		cout << "<식품 : 1 | 옷 : 2 | 가전제품 : 3>" << endl;
		cout << "선택 : ";
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
			cout << "잘못 입력했습니다. 다시 입력하세요." << endl;
			continue;
		}
		break;
	}
}