#include "Manager.h"

const char* CManager::mSubjectName[] = { "C++", "C#" , "API" };

CManager::CManager() {
	for (int i = 0; i < MAXSTUDENT; i++) {
		mStuLst[i] = nullptr;
	}
	mCount = 0;
}

CManager::~CManager() {
	for (int i = 0; i < mCount; i++)
		delete mStuLst[i];
}

void CManager::Input() {
	char name[NAMESIZE];
	char id[IDSIZE];
	int score;

	cout << "성적 입력" << endl;

	cout << "이름 : ";	cin >> name;
	cout << "학번 : ";	cin >> id;

	mStuLst[mCount] = new CStudent(name, id);
	for (int i = 0; i < sizeof(mSubjectName) / sizeof(char*); i++) {
		cout << mSubjectName[i] << " : ";
		cin >> score;
		mStuLst[mCount]->SetSubject(score, i);
	}
	mStuLst[mCount]->CalculateGrade();
	mCount++;
}

void CManager::ShowData() {
	for (int i = 0; i < mCount; i++) {
		cout << "이름 : " << mStuLst[i]->GetName() << "\t";
		cout << "학번 : " << mStuLst[i]->GetId() << endl;
		for (int j = 0; j < sizeof(mSubjectName) / sizeof(char*); j++) {
			cout <<  mSubjectName[j] << " : " << mStuLst[i]->GetSubject(j) << "\t";
		}
		cout << "평점 : " << mStuLst[i]->GetGrade() << endl;
		cout << " ========================= " << endl;
	}
}

void CManager::searchData() {
	char id[IDSIZE];
	bool flag = false;

	cout << "<성적출력>" << endl;
	cout << "학번 : ";	cin >> id;

	for (int i = 0; i < mCount; i++) {
		if (!strcmp(mStuLst[i]->GetId(), id)) {
			cout << "이름 : " << mStuLst[i]->GetName() << "\t";
			cout << "학번 : " << mStuLst[i]->GetId() << endl;
			for (int j = 0; j < sizeof(mSubjectName) / sizeof(char*); j++) {
				cout << mSubjectName[j] << " : " << mStuLst[i]->GetSubject(j) << "\t";
			}
			cout << "평점 : " << mStuLst[i]->GetGrade() << endl;
			cout << " ========================= " << endl;
			flag = true;
			break;
		}
	}
	if (!flag) cout << "없는 학번입니다." << endl;
}

void CManager::ShowMenu() {
	cout << "성적 프로그램" << endl;
	cout << "1. 성적 입력" << endl;
	cout << "2. 성적 조회" << endl;
	cout << "3. 성적 출력" << endl;
	cout << "4. 종료" << endl;
	cout << "선택 : ";
}

void CManager::Run() {
	int select;
	while (1) {
		ShowMenu();
		cin >> select;
		cout << " ========================= " << endl;

		switch (select)
		{
		case INPUT:
			Input();
			break;
		case SEARCH:
			searchData();
			break;
		case SHOWDATA:
			ShowData();
			break;
		case EXIT:
			return;

		default:
			cout << "잘못 입력했습니다. 다시 입력하세요." << endl;
		}
		cout << " ========================= " << endl;
	}
}