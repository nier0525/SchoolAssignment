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
	cBook* ptr3 = new cBook("�ڷᱸ��", code, 5);
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
	
	cout << "<���ο� å�� ����մϴ�>" << endl;

	cout << "å �̸� : ";	cin >> name;
	sprintf(code, "10%d", mCount);
	cout << "å �Ǽ� : ";	cin >> count;

	cBook* ptr = new cBook(name, code, count);
	if (name[BOOKNAME] < BOOKSIZE && count < BOOKCOUNT && code[BOOKCODE] < BOOKCODE) {
		mBooklas[mCount++] = ptr;
	}
	else {
		cout << "Error! ������ ��� å�� ������ �ʰ��Ͽ��ų�, �߸� �Է��ϼ̽��ϴ�." << endl;
		delete ptr;
	}
}

void cBookManager::DeleteBook() {
	char code[BOOKCODE];
	bool flug = false;
	cout << "<å�� �����մϴ�>" << endl;

	cout << "�ڵ� : ";	cin >> code;

	for (int i = 0; i < mCount; i++) {
		if (!strcmp(mBooklas[i]->GetCode(), code)) {
			flug = true;
			cout << mBooklas[i]->GetName() << " �� �����մϴ�." << endl;
			delete mBooklas[i];
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
			cout << "å �Ǽ� : " << mBooklas[i]->GetCount() << endl;
			flug = true;
			break;
		}
	}
	if (!flug) {
		cout << "���� �����Դϴ�." << endl;
	}
}

void cBookManager::Output() {
	char code[BOOKCODE];
	int count;
	int temp;
	int result;

	bool flug = false;


	cout << "<å�� �뿩�մϴ�>" << endl;
	cout << "�ڵ� : ";	cin >> code;

	for (int i = 0; i < mCount; i++) {
		if (!strcmp(mBooklas[i]->GetCode(), code)) {
			flug = true;
			cout << "���� å�� �Ǽ� : "; cin >> count;

			temp = mBooklas[i]->GetCount();
			result = mBooklas[i]->GetCount() - count;
			mBooklas[i]->SetCount(result);

			if (count && mBooklas[i]->GetCount() <= 0) {
				cout << mBooklas[i]->GetName() << " �� " << temp << " �� ���Ƚ��ϴ�." << endl;
				mBooklas[i]->SetCount(0);
			}
			else {
				cout << mBooklas[i]->GetName() << " �� "  << count << " �� ���Ƚ��ϴ�. " << endl;
			}
			break;
		}
	}
	if (!flug) {
		cout << "���� �����Դϴ�." << endl;
	}
}

void cBookManager::Return() {
	char code[BOOKCODE];
	int count;
	int temp;
	int temp2;
	int result;

	bool flug = false;


	cout << "<å�� �ݳ��մϴ�>" << endl;
	cout << "�ڵ� : ";	cin >> code;

	for (int i = 0; i < mCount; i++) {
		if (!strcmp(mBooklas[i]->GetCode(), code)) {
			flug = true;
			cout << "�ݳ��� å�� �Ǽ� : ";	cin >> count;

			result = mBooklas[i]->GetCount() + count;
			mBooklas[i]->SetCount(result);

			if (mBooklas[i]->GetCount() > mBooklas[i]->GetBook()) {

				temp2 = mBooklas[i]->GetBook();
				temp = mBooklas[i]->GetCount() - temp2;
				count = count - temp;
				cout << mBooklas[i]->GetName() << " �� " << count << " �� �ݳ��߽��ϴ�." << endl;
				mBooklas[i]->SetCount(temp2);
			}

			else {
				cout << mBooklas[i]->GetName() << " �� " << count << " �� �ݳ��߽��ϴ�." << endl;
			}
			break;
		}
	}
	if (!flug) {
		cout << "���� �����Դϴ�." << endl;
	}
}