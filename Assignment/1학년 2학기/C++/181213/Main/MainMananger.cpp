#include "MainManager.h"

CMainManager::CMainManager()
{
	for (int i = 0; i < 3; i++) { 
		S[i] = nullptr; 
	}
}

void CMainManager::Initialize()
{
	S[DESIGN] = new CDesignManager();
	S[GRAPHIC] = new CGraphicManager();
	S[PROGRAMMING] = new CProgrammingManager();
}

CMainManager::~CMainManager()
{
	for (int i = 0; i < 3; i++) {
		if (S[i] != nullptr) {
			delete S[i];
		}
	}
}

int CMainManager::ShowMenu()
{
	int select;
	
	cout << "<성적프로그램>" << endl;
	cout << "1.성적입력" << endl;
	cout << "2.성적출력" << endl;
	cout << "3.성적조회" << endl;
	cout << "4.종료" << endl;
	cout << "선택:";
	cin >> select;

	return select;

}

void CMainManager::Run()
{
	int select;	

	Initialize();

	while (1)
	{
		select = ShowMenu();
		
		switch (select)
		{
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
			cout << "<프로그램 종료>" << endl;
			return;
			break;

		default:
			cout << "잘못 입력했습니다 다시 입력하세요 " << endl;
			continue;
		}
	}
}

void CMainManager::MajorInput()
{
	while (1)
	{
		cout << "(프로그래밍:1, 그래픽:2, 기획:3)" << endl;
		cout << "선택:";
		int sel;
		cin >> sel;

		S[sel]->Input();

		break;
	}
}
void CMainManager::MajorSearchData()
{
	while (1)
	{
		cout << "(프로그래밍:1, 그래픽:2, 기획:3)" << endl;
		cout << "선택:";
		int sel;
		cin >> sel;

		S[sel]->Search();

		break;
	} 
}
void CMainManager::MajorDisplay()
{
	while (1)
	{
		cout << "(프로그래밍:1, 그래픽:2, 기획:3)" << endl;
		cout << "선택:";
		int sel;
		cin >> sel;

		S[sel]->Display();

		break;
	}
}

