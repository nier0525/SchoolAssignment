#include "BookManager.h"

cBookManager::cBookManager() {
	memset(mBooklas, 0, sizeof(mBooklas));
	mCount = 0;
	initelize();
}

void cBookManager::initelize() {
	char code[BOOKCODE];
	sprintf(code, "10%d", mCount);
	cBook* ptr = new cBook("C++", code, 5);
	mBooklas[mCount++] = ptr;

	sprintf(code, "10%d", mCount);
	cBook* ptr1 = new cBook("C#", code, 5);
	mBooklas[mCount++] = ptr1;
	
	sprintf(code, "10%d", mCount);
	cBook* ptr2 = new cBook("Unity", code, 5);
	mBooklas[mCount++] = ptr2;
	
	sprintf(code, "10%d", mCount);
	cBook* ptr3 = new cBook("자료구조", code, 5);
	mBooklas[mCount++] = ptr3;
	
	sprintf(code, "10%d", mCount);
	cBook* ptr4 = new cBook("Java", code, 5);
	mBooklas[mCount++] = ptr4;
}


cBookManager::~cBookManager() {
	for (int i = 0; i < mCount; i++) {
		if (mBooklas[i] != nullptr) {
			delete mBooklas[i];
		}
	}
}

void cBookManager::NewBook() {
	char name[BOOKNAME];
	int count;
	char code[BOOKCODE];
	
	cout << "<새로운 책을 등록합니다>" << endl;

	cout << "책 이름 : ";	cin >> name;
	sprintf(code, "10%d", mCount);
	cout << "책 권수 : ";	cin >> count;

	cBook* ptr = new cBook(name, code, count);
	if (name[BOOKNAME] < BOOKSIZE && count < BOOKCOUNT && code[BOOKCODE] < BOOKCODE) {
		mBooklas[mCount++] = ptr;
	}
	else {
		cout << "Error! 도서관 허용 책의 개수가 초과하였거나, 잘못 입력하셨습니다." << endl;
		delete ptr;
	}
}

void cBookManager::DeleteBook() {
	char code[BOOKCODE];
	bool flug = false;
	cout << "<책을 삭제합니다>" << endl;

	cout << "코드 : ";	cin >> code;

	for (int i = 0; i < mCount; i++) {
		if (!strcmp(mBooklas[i]->GetCode(), code)) {
			flug = true;
			cout << mBooklas[i]->GetName() << " 를 삭제합니다." << endl;
			delete mBooklas[i];
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
			cout << "책 권수 : " << mBooklas[i]->GetCount() << endl;
			flug = true;
			break;
		}
	}
	if (!flug) {
		cout << "없는 도서입니다." << endl;
	}
}

void cBookManager::Output() {
	char code[BOOKCODE];
	int count;
	int temp;
	int result;

	bool flug = false;


	cout << "<책을 대여합니다>" << endl;
	cout << "코드 : ";	cin >> code;

	for (int i = 0; i < mCount; i++) {
		if (!strcmp(mBooklas[i]->GetCode(), code)) {
			flug = true;
			cout << "빌릴 책의 권수 : "; cin >> count;

			temp = mBooklas[i]->GetCount();
			result = mBooklas[i]->GetCount() - count;
			mBooklas[i]->SetCount(result);

			if (count && mBooklas[i]->GetCount() <= 0) {
				cout << mBooklas[i]->GetName() << " 를 " << temp << " 권 빌렸습니다." << endl;
				mBooklas[i]->SetCount(0);
			}
			else {
				cout << mBooklas[i]->GetName() << " 를 "  << count << " 권 빌렸습니다. " << endl;
			}
			break;
		}
	}
	if (!flug) {
		cout << "없는 도서입니다." << endl;
	}
}

void cBookManager::Return() {
	char code[BOOKCODE];
	int count;
	int temp;
	int temp2;
	int result;

	bool flug = false;


	cout << "<책을 반납합니다>" << endl;
	cout << "코드 : ";	cin >> code;

	for (int i = 0; i < mCount; i++) {
		if (!strcmp(mBooklas[i]->GetCode(), code)) {
			flug = true;
			cout << "반납할 책의 권수 : ";	cin >> count;

			result = mBooklas[i]->GetCount() + count;
			mBooklas[i]->SetCount(result);

			if (mBooklas[i]->GetCount() > mBooklas[i]->GetBook()) {

				temp2 = mBooklas[i]->GetBook();
				temp = mBooklas[i]->GetCount() - temp2;
				count = count - temp;
				cout << mBooklas[i]->GetName() << " 를 " << count << " 권 반납했습니다." << endl;
				mBooklas[i]->SetCount(temp2);
			}

			else {
				cout << mBooklas[i]->GetName() << " 를 " << count << " 권 반납했습니다." << endl;
			}
			break;
		}
	}
	if (!flug) {
		cout << "없는 도서입니다." << endl;
	}
}