#include "MainManager.h"

cMain::cMain() {
	mId = nullptr;
	mBook = nullptr;
}
cMain::~cMain() {
	delete mId;
	delete mBook;
};

void cMain::initialize() {
	mId = new cIDManager();
	mBook = new cBookManager();

	mBook->initelize();
}

void cMain::Menu() {

	initialize();

	while (1) {
		cout << " =============================================== " << endl;
		cout << "< �� �� >" << endl;
		cout << "1. ȸ������" << endl << "2. �α���" << endl << "3. ������ ���" << endl << "4. ����" << endl;

		int sel;
		cout << "���� : ";	cin >> sel;
		cout << " =============================================== " << endl;

		switch (sel) {
		
		case NEW:
			mId->New();
			break;
	
		case ROGIN:
			if (mId->Rogin()) {
				BookMenu();
			}
			break;

		case HOST:
			mBook->HostMode();
			break;
		
		case EXIT:
			cout << "< ���� >" << endl;
			return;
		
		default:
			cout << "�߸� �Է��ϼ̽��ϴ�." << endl;
			continue;
		}
	}
}

void cMain::BookMenu() {

	bool endflug = false;

	while (1) {
		cout << " =============================================== " << endl;
		cout << "< ���� �뿩 >" << endl;
		cout << "1. ���� ��ȸ" << endl << "2. ���� �˻�" << endl << "3. ���� �뿩" << endl <<
			"4. ���� �ݳ�" << endl << "5. �α׾ƿ�" << endl << "6. ȸ��Ż��" << endl;
		cout << "���� : ";
		int sel;
		cin >> sel;
		cout << " =============================================== " << endl;

		switch(sel) {
		case DISPLAY:
			mBook->Display();
			break;
		case BOOKSERACH:
			mBook->Serach();
			break;
		case BOOKINPUT:
			mBook->Output(mId->GetNowUser());
			break;
		case BOOKRETURN:
			mBook->Return(mId->GetNowUser());
			break;
		case ROGOUT:
			mId->Rogout();
			endflug = true;
			break;
		case GETROG:
			mId->DeleteRog();
			endflug = true;
			break;

		default:
			cout << "�߸� �Է��ϼ̽��ϴ�." << endl;
			continue;
		}
		if (endflug) {
			break;
		}
	}
}