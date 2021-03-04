#include "MainManager.h"

cMainManager::cMainManager() {
	mDesignMng = nullptr;
	mGraphicMng = nullptr;
	mProgrammingMng = nullptr;
}

void cMainManager::Initialize() {
	mDesignMng = new cDesignManager();
	mGraphicMng = new cGraphicManager();
	mProgrammingMng = new cProgrammingManager();
}

cMainManager::~cMainManager() {
	if (mDesignMng != nullptr) { delete mDesignMng; }
	if (mGraphicMng != nullptr) { delete mGraphicMng; }
	if (mProgrammingMng != nullptr) { delete mProgrammingMng; }
}

int cMainManager::ShowMenu() {
	int num;

	cout << "<성적 프로그램>" << endl;
	cout << "1. 성적입력    2.성적출력    3.성적조회    4.종료" << endl;
	cout << "선택 : ";
	cin >> num;

	return num;
}

void cMainManager::run() {
	int num;
	Initialize();

	while (1) {
		num = ShowMenu();
		switch (num) {
		case INPUT:
			MajorInput();
			break;
		case SHOWDATA:
			MajorDisplay();
			break;
		case SEARCHDATA:
			MajorSearchData();
			break;
		case EXIT:
			return;
			break;
		default:
			cout << "잘못 입력했습니다." << endl;
		}
	}
}

void cMainManager::MajorSearchData() {
	while (1) {
		cout << "<프로그래밍 : 1 | 그래픽 : 2 | 기획 : 3>" << endl;
		int num;
		cout << "선택 : ";	cin >> num;

		switch (num) {
		case PROGRAMMING:
			mProgrammingMng->Search();
			break;
		case GRAPHIC:
			mGraphicMng->Search();
			break;
		case DESIGN:
			mDesignMng->Search();
			break;
		default:
			cout << "잘못 입력했습니다" << endl;
			continue;
		}
		break;
	}
}

void cMainManager::MajorDisplay() {
	while (1) {
		cout << "<프로그래밍 : 1 | 그래픽 : 2 | 기획 : 3>" << endl;
		int num;
		cout << "선택 : ";	cin >> num;

		switch (num) {
		case PROGRAMMING:
			mProgrammingMng->Display();
			break;
		case GRAPHIC:
			mGraphicMng->Display();
			break;
		case DESIGN:
			mDesignMng->Display();
			break;
		default:
			cout << "잘못 입력했습니다" << endl;
			continue;
		}
		break;
	}
}

void cMainManager::MajorInput() {
	while (1) {
		cout << "<프로그래밍 : 1 | 그래픽 : 2 | 기획 : 3>" << endl;
		int num;
		cout << "선택 : ";	cin >> num;

		switch (num) {
		case PROGRAMMING:
			mProgrammingMng->Input();
			break;
		case GRAPHIC:
			mGraphicMng->Input();
			break;
		case DESIGN:
			mDesignMng->Input();
			break;
		default:
			cout << "잘못 입력했습니다" << endl;
			continue;
		}
		break;
	}
}