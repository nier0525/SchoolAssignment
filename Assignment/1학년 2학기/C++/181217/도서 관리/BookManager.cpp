#include "BookManager.h"

cBookManager::cBookManager() {
	memset(mBooklas, 0, sizeof(mBooklas));
	mCount = 0;
}

void cBookManager::initelize() {
	mBooklas[mCount++] = new cBook("C++", 5);
	mBooklas[mCount++] = new cBook("C#", 10);
	mBooklas[mCount++] = new cBook("Unity", 20);
	mBooklas[mCount++] = new cBook("TCP/IP", 5);
	mBooklas[mCount++] = new cBook("Java", 12);

	for (int i = 0; i < mCount; i++) {
		mBooklas[i]->Initialize();
	}
}


cBookManager::~cBookManager() {
	for (int i = 0; i < mCount; i++) {
		delete mBooklas[i];
	}
}

void cBookManager::Host() {

	while (1) {
		bool flug = false;
		cout << " =============================================== " << endl;
		cout << "< ������ ��� >" << endl;
		cout << "1. ���� �߰�" << endl << "2. ���� ����" << endl << "3. ������ ��� ����" << endl;
		cout << "���� : ";
		int sel;
		cin >> sel;
		cout << " =============================================== " << endl;

		switch (sel) {
		case INPUT:
			NewBook();
			break;
		case OUTPUT:
			DeleteBook();
			break;
		case RETURN:
			flug = true;
			break;
		}
		if (flug) {
			break;
		}
	}
}

void cBookManager::HostMode() {
	const char* _id = "host";
	const char* _pass = "1234";

	char id[ID];
	char pass[PASSWORD];

	cout << "ID : ";	cin >> id;
	cout << "PASSWORD : ";	cin >> pass;

	if (strcmp(_id, id) == 0 && strcmp(_pass, pass) == 0) {
		Host();
	}
	else {
		cout << "ID �Ǵ� PASSWORD �� Ʋ�Ƚ��ϴ�." << endl;
	}
}

void cBookManager::NewBook() {
	char name[BOOKNAME];
	int count;
	
	cout << "<���ο� å�� ����մϴ�>" << endl;

	cout << "å �̸� : ";	cin >> name;
	cout << "å �Ǽ� : ";	cin >> count;

	cBook* ptr = new cBook(name, count);
	ptr->Initialize();
	mBooklas[mCount++] = ptr;
}

void cBookManager::DeleteBook() {
	char code[BOOKCODE];

	Display();

	bool flug = false;

	cout << "<å�� �����մϴ�>" << endl;

	cout << "�ڵ� : ";	cin >> code;

	for (int i = 0; i < mCount; i++) {
		if (!strcmp(mBooklas[i]->GetCode(), code)) {
			cout << mBooklas[i]->GetName() << " �� �����մϴ�." << endl;
			delete mBooklas[i];
			for (int j = i; j < mCount - 1; j++) {
				mBooklas[j] = mBooklas[j + 1];
			}
			mBooklas[mCount - 1] = nullptr;
			mCount--;
			flug = true;
			break;
		}
	}
	if (!flug) {
		cout << "���� �����Դϴ�." << endl;
	}
}

void cBookManager::Serach() {

	char name[BOOKSIZE];
	bool flug = false;

	cout << "<å�� �˻��մϴ�>" << endl;
	cout << "å �̸� : ";	cin >> name;

	for (int i = 0; i < mCount; i++) {
		if (!strcmp(mBooklas[i]->GetName(), name)) {
			cout << "å �ڵ� : " << mBooklas[i]->GetCode() << endl;
			cout << "å �Ǽ� : " << mBooklas[i]->GetBookCount() << endl;
			flug = true;
			break;
		}
	}
	if (!flug) {
		cout << "���� �����Դϴ�." << endl;
	}
}

void cBookManager::Output(cIDMenu* _user) {
	char code[BOOKCODE];
	int count;
	
	cout << "<å�� �뿩�մϴ�>" << endl;
	cout << "�ڵ� : ";	cin >> code;
	cout << "�Ǽ� : ";	cin >> count;

	bool flug = false;
	for (int i = 0; i < mCount; i++) {
		if (!strcmp(mBooklas[i]->GetCode(), code)) 
		{
			flug = true;
			if (!(mBooklas[i]->CheckLendAble(count) && _user->CheckBorrowAble(count))) 
			{
				cout << "�뿩 ������ �Ǽ��� �ʰ��߰ų�, �뿩�� ���Ͻô� å�� ���� ��� �����ϴ�." << endl;
			}
			else {
				mBooklas[i]->LendBook(_user, count);
				_user->BorrowBook(code, count);
			}
			break;
		}
	}
	if (!flug) {
		cout << "���� �����Դϴ�." << endl;
	}
}

void cBookManager::Return(cIDMenu* _user) {
	char code[BOOKCODE];
	int count;

	bool flug = false;


	cout << "<å�� �ݳ��մϴ�>" << endl;
	cout << "�ڵ� : ";	cin >> code;
	cout << "�ݳ��� å�� �Ǽ� : ";	cin >> count;

	for (int i = 0; i < mCount; i++) {
		if (!strcmp(mBooklas[i]->GetCode(), code)) {
			flug = true;

			if (!(mBooklas[i]->CheckReturnAble(_user,count) && _user->CheckReturnAble(code,count))) {
				cout << "�ݳ� ������ �߸��Ǿ����ϴ�." << endl;
			}
			else {
				mBooklas[i]->ReturnBook(_user, count);
				_user->ReturnBook(code, count);
			}
			break;
		}
	}
	if (!flug) {
		cout << "���� �����Դϴ�." << endl;
	}
}

void cBookManager::Display() {
	for (int i = 0; i < mCount; i++) {
		cout << "������ : " << mBooklas[i]->GetName() << endl;
		cout << "�ڵ� : " << mBooklas[i]->GetCode() << endl;
		cout << "���� ��� : " << mBooklas[i]->GetBook() << endl;
	}
}