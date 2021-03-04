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

	cout << "<���� ���α׷�>" << endl;
	cout << "1. �����Է�    2.�������    3.������ȸ    4.����" << endl;
	cout << "���� : ";
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
			cout << "�߸� �Է��߽��ϴ�." << endl;
		}
	}
}

void cMainManager::MajorSearchData() {
	while (1) {
		cout << "<���α׷��� : 1 | �׷��� : 2 | ��ȹ : 3>" << endl;
		int num;
		cout << "���� : ";	cin >> num;

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
			cout << "�߸� �Է��߽��ϴ�" << endl;
			continue;
		}
		break;
	}
}

void cMainManager::MajorDisplay() {
	while (1) {
		cout << "<���α׷��� : 1 | �׷��� : 2 | ��ȹ : 3>" << endl;
		int num;
		cout << "���� : ";	cin >> num;

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
			cout << "�߸� �Է��߽��ϴ�" << endl;
			continue;
		}
		break;
	}
}

void cMainManager::MajorInput() {
	while (1) {
		cout << "<���α׷��� : 1 | �׷��� : 2 | ��ȹ : 3>" << endl;
		int num;
		cout << "���� : ";	cin >> num;

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
			cout << "�߸� �Է��߽��ϴ�" << endl;
			continue;
		}
		break;
	}
}