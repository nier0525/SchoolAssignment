#include "DesighManager.h"
const char* cDesignManager::mSubjectName[] = { "게임기획기초","게임시나리오","레벨디자인" };

cDesignManager::cDesignManager() {}
cDesignManager::~cDesignManager() {}

void cDesignManager::Input() {
	char name[NAMESIZE];
	char id[IDSIZE];
	int score;

	cout << "이름 : ";
	cin >> name;

	sprintf_s(id, "2017%s%03d", PROGRAM_CODE, mStuCount + 1);
	cStudent* ptr = new cStudent(name, id);

	for (int i = 0; i < sizeof(mSubjectName) / sizeof(char*); i++) {
		cout << mSubjectName[i] << " : ";
		cin >> score;
		ptr->SetSubject(score, i);
	}
	mStudentList[mStuCount++] = ptr;
}

void cDesignManager::Display() {
	for (int i = 0; i < mStuCount; i++) {
		cout << "이름 : " << mStudentList[i]->GetName() << "\t";
		cout << "학번 : " << mStudentList[i]->GetId() << endl;

		for (int j = 0; j < sizeof(mSubjectName) / sizeof(char*); j++) {
			cout << mSubjectName[j] << " : ";
			cout << mStudentList[i]->GetSubject(j) << "\t";
		}
		mStudentList[i]->CalculateGrade();

		cout << "평점 : ";
		cout << mStudentList[i]->GetGrade() << endl;
	}
}

void cDesignManager::Search() {
	char id[IDSIZE];
	cout << "학번 : ";
	cin >> id;

	for (int i = 0; i < mStuCount; i++) {
		if (!strcmp(mStudentList[i]->GetId(), id)) {
			cout << "이름 : " << mStudentList[i]->GetName() << "\t";
			cout << "학번 : " << mStudentList[i]->GetId() << endl;

			for (int j = 0; j < sizeof(mSubjectName) / sizeof(char*); j++) {
				cout << mSubjectName[j] << " : ";
				cout << mStudentList[i]->GetSubject(j) << "\t";
			}
			//	mStudentList[i]->CalculateGrade();

			cout << "평점 : ";
			cout << mStudentList[i]->GetGrade() << endl;
			break;
		}
	}
}