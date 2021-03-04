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
		cout << "< 관리자 모드 >" << endl;
		cout << "1. 도서 추가" << endl << "2. 도서 삭제" << endl << "3. 관리자 모드 종료" << endl;
		cout << "선택 : ";
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
		cout << "ID 또는 PASSWORD 가 틀렸습니다." << endl;
	}
}

void cBookManager::NewBook() {
	char name[BOOKNAME];
	int count;
	
	cout << "<새로운 책을 등록합니다>" << endl;

	cout << "책 이름 : ";	cin >> name;
	cout << "책 권수 : ";	cin >> count;

	cBook* ptr = new cBook(name, count);
	ptr->Initialize();
	mBooklas[mCount++] = ptr;
}

void cBookManager::DeleteBook() {
	char code[BOOKCODE];

	Display();

	bool flug = false;

	cout << "<책을 삭제합니다>" << endl;

	cout << "코드 : ";	cin >> code;

	for (int i = 0; i < mCount; i++) {
		if (!strcmp(mBooklas[i]->GetCode(), code)) {
			cout << mBooklas[i]->GetName() << " 를 삭제합니다." << endl;
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
		cout << "없는 도서입니다." << endl;
	}
}

void cBookManager::Serach() {

	char name[BOOKSIZE];
	bool flug = false;

	cout << "<책을 검색합니다>" << endl;
	cout << "책 이름 : ";	cin >> name;

	for (int i = 0; i < mCount; i++) {
		if (!strcmp(mBooklas[i]->GetName(), name)) {
			cout << "책 코드 : " << mBooklas[i]->GetCode() << endl;
			cout << "책 권수 : " << mBooklas[i]->GetBookCount() << endl;
			flug = true;
			break;
		}
	}
	if (!flug) {
		cout << "없는 도서입니다." << endl;
	}
}

void cBookManager::Output(cIDMenu* _user) {
	char code[BOOKCODE];
	int count;
	
	cout << "<책을 대여합니다>" << endl;
	cout << "코드 : ";	cin >> code;
	cout << "권수 : ";	cin >> count;

	bool flug = false;
	for (int i = 0; i < mCount; i++) {
		if (!strcmp(mBooklas[i]->GetCode(), code)) 
		{
			flug = true;
			if (!(mBooklas[i]->CheckLendAble(count) && _user->CheckBorrowAble(count))) 
			{
				cout << "대여 가능한 권수가 초과했거나, 대여를 원하시는 책이 현재 재고가 없습니다." << endl;
			}
			else {
				mBooklas[i]->LendBook(_user, count);
				_user->BorrowBook(code, count);
			}
			break;
		}
	}
	if (!flug) {
		cout << "없는 도서입니다." << endl;
	}
}

void cBookManager::Return(cIDMenu* _user) {
	char code[BOOKCODE];
	int count;

	bool flug = false;


	cout << "<책을 반납합니다>" << endl;
	cout << "코드 : ";	cin >> code;
	cout << "반납할 책의 권수 : ";	cin >> count;

	for (int i = 0; i < mCount; i++) {
		if (!strcmp(mBooklas[i]->GetCode(), code)) {
			flug = true;

			if (!(mBooklas[i]->CheckReturnAble(_user,count) && _user->CheckReturnAble(code,count))) {
				cout << "반납 정보가 잘못되었습니다." << endl;
			}
			else {
				mBooklas[i]->ReturnBook(_user, count);
				_user->ReturnBook(code, count);
			}
			break;
		}
	}
	if (!flug) {
		cout << "없는 도서입니다." << endl;
	}
}

void cBookManager::Display() {
	for (int i = 0; i < mCount; i++) {
		cout << "도서명 : " << mBooklas[i]->GetName() << endl;
		cout << "코드 : " << mBooklas[i]->GetCode() << endl;
		cout << "현재 재고 : " << mBooklas[i]->GetBook() << endl;
	}
}