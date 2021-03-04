#include "MainManager.h"

char cMain::_ID[] = { "koreait" };
char cMain::_PASS[] = { "1234" };

cMain::cMain() {
	mId = nullptr;
	mBook = nullptr;
}
cMain::~cMain() {
	if (mId != nullptr) {
		delete mId;
	}
	if (mBook != nullptr) {
		delete mBook;
	}
};

void cMain::initialize() {
	mId = new cIDManager();
	mBook = new cBookManager();
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
			ManagerID();
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
		cout << "1. ���� �˻�" << endl << "2. ���� �뿩" << endl << "3. ���� �ݳ�" << endl << "4. �α׾ƿ�" << endl
			<< "5. ȸ��Ż��" << endl;
		cout << "���� : ";
		int sel;
		cin >> sel;
		cout << " =============================================== " << endl;

		switch(sel) {
		case BOOKSERACH:
			mBook->Serach();
			break;
		case BOOKINPUT:
			mBook->Output();
			break;
		case BOOKRETURN:
			mBook->Return();
			break;
		case ROGOUT:
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

void cMain::ManagerID() {
	char id[ID];
	char pass[ID];

	cout << "ID : ";	cin >> id;
	cout << "PASSWORD : ";	cin >> pass;

	if (!strcmp(_ID, id) && !strcmp(_PASS, pass)) {
		while (1) {
			cout << " =============================================== " << endl;
			cout << "< ������ �޴� >" << endl << "1. ���� �߰�" << endl << "2. ���� ����" << endl << "3. ����" << endl;
			cout << "���� : ";
			int sel;
			cin >> sel;
			cout << " =============================================== " << endl;

			bool endflug = false;

			switch (sel)
			{
			case INPUT:
				mBook->NewBook();
				break;
			case OUTPUT:
				mBook->DeleteBook();
				break;
			case RETURN:
				endflug = true;
				break;
			default:
				cout << "�߸� �Է��ϼ̽��ϴ�." << endl;
				continue;
			}
			if (endflug) {
				break;
			}
			break;
		}
	}
	else {
		cout << "ID �Ǵ� PASSWORD �� ���� �ʽ��ϴ�." << endl;
	}
}