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
	
	cout << "<�������α׷�>" << endl;
	cout << "1.�����Է�" << endl;
	cout << "2.�������" << endl;
	cout << "3.������ȸ" << endl;
	cout << "4.����" << endl;
	cout << "����:";
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
			cout << "<���α׷� ����>" << endl;
			return;
			break;

		default:
			cout << "�߸� �Է��߽��ϴ� �ٽ� �Է��ϼ��� " << endl;
			continue;
		}
	}
}

void CMainManager::MajorInput()
{
	while (1)
	{
		cout << "(���α׷���:1, �׷���:2, ��ȹ:3)" << endl;
		cout << "����:";
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
		cout << "(���α׷���:1, �׷���:2, ��ȹ:3)" << endl;
		cout << "����:";
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
		cout << "(���α׷���:1, �׷���:2, ��ȹ:3)" << endl;
		cout << "����:";
		int sel;
		cin >> sel;

		S[sel]->Display();

		break;
	}
}

